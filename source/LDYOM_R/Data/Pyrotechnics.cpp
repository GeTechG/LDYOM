#include "Pyrotechnics.h"

#include <array>
#include <CStreaming.h>
#include <CTheScripts.h>
#include <CWorld.h>
#include <boost/uuid/random_generator.hpp>
#include <extensions/ScriptCommands.h>
#include <extensions/scripting/ScriptCommandNames.h>

#include "CFireManager.h"
#include "components.h"
#include "ModelsService.h"
#include "ProjectPlayerService.h"
#include "Scene.h"
#include "strUtils.h"
#include "utils.h"
#include "easylogging/easylogging++.h"

using namespace plugin;

std::optional<int> Pyrotechnics::spawnFire(bool editor) {
	int scriptFire;
	Command<Commands::START_SCRIPT_FIRE>(this->pos_[0], this->pos_[1], this->pos_[2],
	                                     editor ? 0 : this->propagationFire_, this->sizeFire_, &scriptFire);

	return scriptFire;
}

Pyrotechnics::Pyrotechnics(const char *name, const CVector &pos): ObjectiveDependent(nullptr),
                                                                  uuid_(boost::uuids::random_generator()()) {
	this->name_ = name;
	this->pos_[0] = pos.x;
	this->pos_[1] = pos.y;
	this->pos_[2] = pos.z;
}

Pyrotechnics Pyrotechnics::copy() const {
	Pyrotechnics copy(*this);
	copy.uuid_ = boost::uuids::random_generator()();
	copy.name_ += " (copy)";
	copy.editorFire_ = std::nullopt;
	copy.projectFire_ = std::nullopt;
	copy.editorExplosionObject_ = std::nullopt;

	return copy;
}

Pyrotechnics::~Pyrotechnics() {
	this->deleteEditorPyrotechnics();
	this->deleteProjectEntity();
}

boost::uuids::uuid& Pyrotechnics::getUuid() {
	return uuid_;
}

std::optional<CFire*> Pyrotechnics::getEditorFire() {
	if (this->editorFire_.has_value()) {
		const int actualScriptThingIndex = CTheScripts::GetActualScriptThingIndex(this->editorFire_.value(), 5);
		const auto fire = &gFireManager.m_aFires[actualScriptThingIndex];
		if (fire->m_nFlags.bActive != 0) {
			return fire;
		}
	}
	return std::nullopt;
}

std::optional<CFire*> Pyrotechnics::getProjectFire() {
	if (this->projectFire_.has_value()) {
		const int actualScriptThingIndex = CTheScripts::GetActualScriptThingIndex(this->projectFire_.value(), 5);
		const auto fire = &gFireManager.m_aFires[actualScriptThingIndex];
		if (fire->m_nFlags.bActive != 0) {
			return fire;
		}
	}
	return std::nullopt;
}

int& Pyrotechnics::getType() {
	return type_;
}

int& Pyrotechnics::getTypeExplosion() {
	return typeExplosion_;
}

int& Pyrotechnics::getSizeFire() {
	return sizeFire_;
}

int& Pyrotechnics::getPropagationFire() {
	return propagationFire_;
}

std::optional<int>& Pyrotechnics::getProjectFireId() {
	return this->projectFire_;
}

void Pyrotechnics::updateLocation() {
	if (const auto fire = this->getEditorFire(); fire.has_value()) {
		fire.value()->m_vecPosition = {this->pos_[0], this->pos_[1], this->pos_[2]};
		CMatrix newMatrix(&fire.value()->m_pFxSystem->m_localMatrix, true);
		newMatrix.SetTranslateOnly(this->pos_[0], this->pos_[1], this->pos_[2]);
		newMatrix.UpdateRW(&fire.value()->m_pFxSystem->m_localMatrix);
	}
	if (this->editorExplosionObject_.has_value()) {
		CWorld::Remove(this->editorExplosionObject_.value());
		this->editorExplosionObject_.value()->SetPosn(this->pos_[0], this->pos_[1], this->pos_[2]);
		this->editorExplosionObject_.value()->UpdateRwFrame();
		CWorld::Add(this->editorExplosionObject_.value());
	}

	if (const auto fire = this->getProjectFire(); fire.has_value()) {
		fire.value()->m_vecPosition = {this->pos_[0], this->pos_[1], this->pos_[2]};
		CMatrix newMatrix(&fire.value()->m_pFxSystem->m_localMatrix, true);
		newMatrix.SetTranslateOnly(this->pos_[0], this->pos_[1], this->pos_[2]);
		newMatrix.UpdateRW(&fire.value()->m_pFxSystem->m_localMatrix);
	}
}

std::string& Pyrotechnics::getName() {
	return this->name_;
}

float* Pyrotechnics::getPosition() {
	return pos_.data();
}

void Pyrotechnics::spawnEditorPyrotechnics() {
	if (editorExplosionObject_.has_value() || editorFire_.has_value())
		this->deleteEditorPyrotechnics();

	if (this->type_ == 0) {
		this->editorFire_ = spawnFire(true);
		this->getEditorFire().value()->m_nNumGenerationsAllowed = 0;
	} else {
		CStreaming::RequestModel(1654, 0);
		CStreaming::LoadAllRequestedModels(false);

		this->editorExplosionObject_ = CObject::Create(1654);
		this->editorExplosionObject_.value()->SetPosn(this->pos_[0], this->pos_[1], this->pos_[2]);
		this->editorExplosionObject_.value()->m_bUsesCollision = false;
		CWorld::Add(this->editorExplosionObject_.value());

		CStreaming::RemoveAllUnusedModels();
	}
}

extern bool restart;

void Pyrotechnics::deleteEditorPyrotechnics() {
	if (this->editorExplosionObject_.has_value() && !restart) {
		const auto object = this->editorExplosionObject_.value();
		if (CPools::ms_pObjectPool->IsObjectValid(object)) {
			const int objectRef = CPools::GetObjectRef(object);
			Command<Commands::DELETE_OBJECT>(objectRef);
		}
		this->editorExplosionObject_ = std::nullopt;
	}
	if (this->editorFire_.has_value() && !restart) {
		const int actualScriptThingIndex = CTheScripts::GetActualScriptThingIndex(this->editorFire_.value(), 5);
		if (actualScriptThingIndex != -1) {
			gFireManager.RemoveScriptFire(static_cast<short>(actualScriptThingIndex));
		}
		this->editorFire_ = std::nullopt;
	}
}

void Pyrotechnics::spawnProjectEntity() {
	if (projectFire_.has_value())
		this->deleteProjectEntity();

	if (this->type_ == 0) {
		this->projectFire_ = spawnFire();
	} else {
		Command<Commands::ADD_EXPLOSION>(this->pos_[0], this->pos_[1], this->pos_[2], this->typeExplosion_);
	}

	updateLocation();
}

void Pyrotechnics::deleteProjectEntity() {
	if (this->projectFire_.has_value() && !restart) {
		const int actualScriptThingIndex = CTheScripts::GetActualScriptThingIndex(this->projectFire_.value(), 5);
		if (actualScriptThingIndex != -1) {
			gFireManager.RemoveScriptFire(static_cast<short>(actualScriptThingIndex));
		}
		this->projectFire_ = std::nullopt;
	}
}
