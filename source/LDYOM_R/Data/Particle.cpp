#include "Particle.h"

#include <array>
#include <CStreaming.h>
#include <boost/uuid/random_generator.hpp>

#include "components.h"
#include "CTheScripts.h"
#include "FxManager_c.h"
#include "LuaEngine.h"
#include "ModelsService.h"
#include "ProjectPlayerService.h"
#include "ProjectsService.h"
#include "Scene.h"
#include "strUtils.h"
#include "utils.h"
#include "easylogging/easylogging++.h"

using namespace plugin;

std::optional<int> Particle::spawnParticle() {
	RwMatrix mat;
	const auto particleName = ModelsService::getInstance().getParticlesNames()[this->particleType_];
	const auto fxSystem = g_fxMan.CreateFxSystem(const_cast<char*>(particleName.c_str()), &mat, nullptr, 1);
	const auto scriptEffectSystem = CTheScripts::AddScriptEffectSystem(fxSystem);
	fxSystem->Play();

	return scriptEffectSystem;
}

Particle::Particle(const char *name, const CVector &pos): ObjectiveDependent(nullptr),
                                                          uuid_(boost::uuids::random_generator()()),
                                                          rotate{{0, 0, 0}, 1} {
	this->name_ = name;
	this->pos_[0] = pos.x;
	this->pos_[1] = pos.y;
	this->pos_[2] = pos.z;
}

Particle Particle::copy() const {
	Particle particle(*this);
	particle.uuid_ = boost::uuids::random_generator()();
	particle.name_ += " (copy)";

	return particle;
}

Particle::~Particle() {
	this->deleteEditorParticle();
	this->deleteProjectEntity();
}

boost::uuids::uuid& Particle::getUuid() {
	return uuid_;
}

std::optional<FxSystem_c*> Particle::getEditorParticle() {
	if (this->editorParticle_.has_value()) {
		auto actual = CTheScripts::GetActualScriptThingIndex(this->editorParticle_.value(), 1);
		const auto fxSystem = static_cast<FxSystem_c*>(CTheScripts::ScriptEffectSystemArray[actual].m_pFxSystem);
		if (fxSystem != nullptr)
			return fxSystem;
	}
	return std::nullopt;
}

std::optional<FxSystem_c*> Particle::getProjectParticle() {
	if (this->projectParticle_.has_value()) {
		auto actual = CTheScripts::GetActualScriptThingIndex(this->projectParticle_.value(), 1);
		const auto fxSystem = static_cast<FxSystem_c*>(CTheScripts::ScriptEffectSystemArray[actual].m_pFxSystem);
		if (fxSystem != nullptr)
			return fxSystem;
	}
	return std::nullopt;
}

CQuaternion& Particle::getRotations() {
	return this->rotate;
}

std::array<float, 3>& Particle::getScale() {
	return scale_;
}

int& Particle::getParticleType() {
	return particleType_;
}

int& Particle::getAttachType() {
	return attachType_;
}

boost::uuids::uuid& Particle::getAttachUuid() {
	return attachUuid_;
}

int& Particle::getPedBodeId() {
	return pedBodeId_;
}

std::optional<int>& Particle::getProjectParticleId() {
	return this->projectParticle_;
}


void Particle::updateLocation() {
	CQuaternion rw;
	rw.real = this->rotate.real;
	rw.imag.x = this->rotate.imag.z;
	rw.imag.y = this->rotate.imag.x;
	rw.imag.z = -this->rotate.imag.y;
	CMatrix matrix;

	if (const auto editorParticle = this->getEditorParticle(); editorParticle.has_value()) {
		matrix.SetRotate(rw);
		matrix.SetTranslateOnly(this->pos_[0], this->pos_[1], this->pos_[2]);
		matrix.right *= this->scale_[2];
		matrix.up *= this->scale_[0];
		matrix.at *= this->scale_[1];
		matrix.UpdateRW(&editorParticle.value()->m_localMatrix);
	}
	if (const auto projectParticle = this->getProjectParticle(); projectParticle.has_value()) {
		RwMatrix *parent;
		switch (this->attachType_) {
			case 1: {
				const auto &actors = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getActors();
				const int index = utils::indexByUuid(actors, this->attachUuid_);
				if (index != -1) {
					const auto &actor = ProjectsService::getInstance().
					                    getCurrentProject().getCurrentScene()->getActors().
					                    at(index);

					auto animHier = GetAnimHierarchyFromSkinClump(actor->getProjectPed().value()->m_pRwClump);
					auto boneIndex = RpHAnimIDGetIndex(animHier, this->pedBodeId_);
					parent = &RpHAnimHierarchyGetMatrixArray(animHier)[boneIndex];
					actor->getProjectPed().value()->Update();
					actor->getProjectPed().value()->UpdateAnim();
					actor->getProjectPed().value()->UpdateRpHAnim();
					actor->getProjectPed().value()->UpdateRwFrame();

					CQuaternion parentQuat;
					parentQuat.Set(*parent);
					parentQuat.Normalise();
					parentQuat.Conjugate();
					CMatrix parentNewMat;
					parentNewMat.SetRotate(parentQuat);

					CQuaternion newRw;
					newRw.Multiply(parentQuat, rw);

					matrix.SetRotate(newRw);
					CVector pos = {
						this->getPosition()[0] - parent->pos.x,
						this->getPosition()[1] - parent->pos.y,
						this->getPosition()[2] - parent->pos.z
					};
					CVector newPos;
					newPos.FromMultiply3x3(parentNewMat, pos);
					matrix.SetTranslateOnly(newPos.x, newPos.y, newPos.z);
				}
				break;
			}
			case 2: {
				const auto &vehicles = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->
				                                                      getVehicles();
				const int index = utils::indexByUuid(vehicles, this->attachUuid_);
				if (index != -1) {
					const auto &vehicle = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->
					                                                     getVehicles().at(index);
					parent = reinterpret_cast<RwMatrix*>(vehicle->getProjectVehicle().value()->GetMatrix());

					matrix.SetRotate(rw);
					matrix.RotateZ(RAD(-vehicle->getHeadingAngle()));
					float x = this->getPosition()[0] - vehicle->getPosition()[0];
					float y = this->getPosition()[1] - vehicle->getPosition()[1];
					float z = this->getPosition()[2] - vehicle->getPosition()[2];
					rotateVec2(x, y, -vehicle->getHeadingAngle());
					matrix.SetTranslateOnly(x, y, z);
				}
				break;
			}
			case 3: {
				const auto &objects = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->
				                                                     getObjects();
				const int index = utils::indexByUuid(objects, this->attachUuid_);
				if (index != -1) {
					const auto &object = ProjectsService::getInstance().
					                     getCurrentProject().getCurrentScene()->getObjects().
					                     at(index);
					parent = reinterpret_cast<RwMatrix*>(object->getProjectObject().value()->GetMatrix());
					RwMatrix parentMatRot;
					CQuaternion parentQuat;
					parentQuat.real = object->getRotations().real;
					parentQuat.imag.x = object->getRotations().imag.z;
					parentQuat.imag.y = object->getRotations().imag.x;
					parentQuat.imag.z = -object->getRotations().imag.y;
					parentQuat.Normalise();
					parentQuat.Conjugate();
					parentQuat.Get(&parentMatRot);
					CQuaternion newRw;
					newRw.Multiply(parentQuat, rw);
					CVector pos = {
						this->getPosition()[0] - object->getPosition()[0],
						this->getPosition()[1] - object->getPosition()[1],
						this->getPosition()[2] - object->getPosition()[2]
					};
					CVector rotatedPos;
					rotatedPos.FromMultiply3x3(reinterpret_cast<CMatrix&>(parentMatRot), pos);

					matrix.SetRotate(newRw);
					matrix.SetTranslateOnly(rotatedPos.x, rotatedPos.y, rotatedPos.z);
				}
				break;
			}
			default:
				matrix.SetRotate(rw);
				matrix.SetTranslateOnly(this->getPosition()[0], this->getPosition()[1], this->getPosition()[2]);
				break;
		}

		matrix.right *= this->scale_[2];
		matrix.up *= this->scale_[0];
		matrix.at *= this->scale_[1];

		matrix.UpdateRW(&projectParticle.value()->m_localMatrix);
		if (this->attachType_ > 0)
			projectParticle.value()->m_pParentMatrix = parent;
	}
}

std::string& Particle::getName() {
	return this->name_;
}

float* Particle::getPosition() {
	return pos_.data();
}

void Particle::spawnEditorParticle() {
	if (editorParticle_.has_value())
		this->deleteEditorParticle();

	this->editorParticle_ = spawnParticle();

	updateLocation();
}

extern bool restart;

void Particle::deleteEditorParticle() {
	const auto fxSystemC = this->getEditorParticle();
	if (fxSystemC.has_value() && !restart) {
		g_fxMan.DestroyFxSystem(fxSystemC.value());
		CTheScripts::RemoveScriptEffectSystem(this->editorParticle_.value());
	}
	this->editorParticle_ = std::nullopt;
}

void Particle::spawnProjectEntity() {
	if (projectParticle_.has_value())
		this->deleteProjectEntity();

	this->projectParticle_ = spawnParticle();

	updateLocation();

	auto scene = ProjectPlayerService::getInstance().getCurrentScene();
	auto tasklist = ProjectPlayerService::getInstance().getSceneTasklist();

	if (scene.has_value() && tasklist != nullptr) {
		/*const auto onParticleSpawn = LuaEngine::getInstance().getLuaState()["global_data"]["signals"]["onParticleSpawn"]
			.get_or_create<sol::table>();
		for (auto [_, func] : onParticleSpawn) {
			if (const auto result = func.as<sol::function>()(scene.value(), tasklist, this->uuid_); !result.valid()) {
				const sol::error err = result;
				CLOG(ERROR, "lua") << err.what();
			}
		}*/
	}
}

void Particle::deleteProjectEntity() {
	const auto fxSystemC = this->getProjectParticle();
	if (fxSystemC.has_value() && !restart) {
		g_fxMan.DestroyFxSystem(fxSystemC.value());
		CTheScripts::RemoveScriptEffectSystem(this->projectParticle_.value());
	}
	this->projectParticle_ = std::nullopt;

	auto scene = ProjectPlayerService::getInstance().getCurrentScene();
	auto tasklist = ProjectPlayerService::getInstance().getSceneTasklist();

	if (scene.has_value() && tasklist != nullptr) {
		/*const auto onParticleDelete = LuaEngine::getInstance().getLuaState()["global_data"]["signals"][
			"onParticleDelete"].get_or_create<sol::table>();
		for (auto [_, func] : onParticleDelete) {
			if (const auto result = func.as<sol::function>()(scene.value(), tasklist, this->uuid_); !result.valid()) {
				const sol::error err = result;
				CLOG(ERROR, "lua") << err.what();
			}
		}*/
	}
}
