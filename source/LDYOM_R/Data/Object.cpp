#include "Object.h"

#include <array>
#include <CPools.h>
#include <CStreaming.h>
#include <CWorld.h>
#include <boost/uuid/random_generator.hpp>
#include <extensions/ScriptCommands.h>
#include <extensions/scripting/ScriptCommandNames.h>

#include "components.h"
#include "LuaEngine.h"
#include "ModelsService.h"
#include "ProjectPlayerService.h"
#include "Scene.h"
#include "strUtils.h"
#include "utils.h"
#include "easylogging/easylogging++.h"

using namespace plugin;

std::optional<CObject*> Object::spawnObject() {
	if (!ModelsService::getInstance().validObjectModel(this->modelId))
		return std::nullopt;

	CStreaming::RequestModel(this->modelId, GAME_REQUIRED);
	CStreaming::LoadAllRequestedModels(false);

	int newObjectHandle;
	Command<Commands::CREATE_OBJECT_NO_OFFSET>(this->modelId, this->pos[0], this->pos[1], this->pos[2],
	                                           &newObjectHandle);
	const auto newObject = CPools::GetObject(newObjectHandle);

	CStreaming::SetMissionDoesntRequireModel(this->modelId);

	return newObject;
}

Object::Object(const char *name, const CVector &pos): ObjectiveDependent(nullptr),
                                                      uuid(boost::uuids::random_generator()()),
                                                      rotate{{0, 0, 0}, 1},
                                                      modelId(325) {
	this->name = name;
	this->pos[0] = pos.x;
	this->pos[1] = pos.y;
	this->pos[2] = pos.z;
}

Object Object::copy() const {
	Object copy(*this);
	copy.name += " (copy)";
	copy.uuid = boost::uuids::random_generator()();

	return copy;
}

Object::~Object() {
	this->deleteEditorObject();
	this->deleteProjectEntity();
}

boost::uuids::uuid& Object::getUuid() {
	return uuid;
}

std::optional<CObject*>& Object::getEditorObject() {
	return editorObject_;
}

std::optional<CObject*>& Object::getProjectObject() {
	return projectObject_;
}

CQuaternion& Object::getRotations() {
	return this->rotate;
}

std::array<float, 3>& Object::getScale() {
	return scale;
}

int& Object::getModelId() {
	return modelId;
}

void Object::updateLocation() {
	CQuaternion rw;
	rw.real = this->rotate.real;
	rw.imag.x = this->rotate.imag.z;
	rw.imag.y = this->rotate.imag.x;
	rw.imag.z = -this->rotate.imag.y;
	CMatrix newMatrix;
	newMatrix.SetRotate(rw);
	newMatrix.SetTranslateOnly(this->pos[0], this->pos[1], this->pos[2]);
	newMatrix.right *= this->scale[2];
	newMatrix.up *= this->scale[0];
	newMatrix.at *= this->scale[1];

	if (this->editorObject_.has_value()) {
		CWorld::Remove(this->editorObject_.value());
		this->editorObject_.value()->SetMatrix(newMatrix);
		this->editorObject_.value()->UpdateRwFrame();
		CWorld::Add(this->editorObject_.value());
	}
	if (this->projectObject_.has_value()) {
		CWorld::Remove(this->projectObject_.value());
		this->projectObject_.value()->SetMatrix(newMatrix);
		this->projectObject_.value()->UpdateRwFrame();
		CWorld::Add(this->projectObject_.value());
	}
}

std::string& Object::getName() {
	return this->name;
}

float* Object::getPosition() {
	return pos;
}

void Object::spawnEditorObject() {
	if (editorObject_.has_value())
		deleteEditorObject();

	editorObject_ = spawnObject();

	const auto editorObjectId = editorObject_.value();
	Command<Commands::FREEZE_OBJECT_POSITION>(editorObjectId, 1);
	Command<Commands::SET_OBJECT_DYNAMIC>(editorObjectId, 0);
	Command<Commands::SET_OBJECT_PROOFS>(editorObjectId, 1, 1, 1, 1, 1);

	updateLocation();
}

extern bool restart;

void Object::deleteEditorObject() {
	if (editorObject_.has_value() && !restart) {
		const auto object = editorObject_.value();
		if (CPools::ms_pObjectPool->IsObjectValid(object)) {
			const int objectRef = CPools::GetObjectRef(object);
			Command<Commands::DELETE_OBJECT>(objectRef);
		}
		editorObject_ = std::nullopt;
	}
}

void Object::spawnProjectEntity() {
	if (projectObject_.has_value())
		deleteProjectEntity();

	projectObject_ = spawnObject();
	updateLocation();

	auto scene = ProjectPlayerService::getInstance().getCurrentScene();
	auto tasklist = ProjectPlayerService::getInstance().getSceneTasklist();

	if (scene.has_value() && tasklist != nullptr) {
		/*const auto onObjectSpawn = LuaEngine::getInstance().getLuaState()["global_data"]["signals"]["onObjectSpawn"].
			get_or_create<sol::table>();
		for (auto func : onObjectSpawn | views::values) {
			if (const auto result = func.as<sol::function>()(scene.value(), tasklist, this->uuid); !result.valid()) {
				const sol::error err = result;
				CLOG(ERROR, "lua") << err.what();
			}
		}*/
	}
}

void Object::deleteProjectEntity() {
	if (projectObject_.has_value() && !restart) {
		if (const auto object = projectObject_.value(); CPools::ms_pObjectPool->IsObjectValid(object)) {
			const int objectRef = CPools::GetObjectRef(object);
			Command<Commands::DELETE_OBJECT>(objectRef);
			projectObject_ = std::nullopt;

			auto scene = ProjectPlayerService::getInstance().getCurrentScene();
			auto tasklist = ProjectPlayerService::getInstance().getSceneTasklist();

			if (scene.has_value() && tasklist != nullptr) {
				/*const auto onObjectDelete = LuaEngine::getInstance().getLuaState()["global_data"]["signals"][
						"onObjectDelete"].
					get_or_create<sol::table>();
				for (const auto &func : onObjectDelete | views::values) {
					if (const auto result = func.as<sol::function>()(scene.value(), tasklist, uuid); !result.valid()) {
						const sol::error err = result;
						CLOG(ERROR, "lua") << err.what();
					}
				}*/
			}
		}
	}
}
