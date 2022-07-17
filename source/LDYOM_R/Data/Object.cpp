#include "Object.h"

#include <array>
#include <CPools.h>
#include <CStreaming.h>
#include <CWorld.h>
#include <boost/uuid/random_generator.hpp>
#include <extensions/ScriptCommands.h>
#include <extensions/scripting/ScriptCommandNames.h>

#include "components.h"
#include "ModelsService.h"
#include "strUtils.h"
#include "utils.h"

using namespace plugin;

std::optional<CObject*> Object::spawnObject() {
	if (!ModelsService::getInstance().validObjectModel(this->modelId_))
		return std::nullopt;

	CStreaming::RequestModel(this->modelId_, GAME_REQUIRED);
	CStreaming::LoadAllRequestedModels(false);

	int newObjectHandle;
	Command<Commands::CREATE_OBJECT_NO_OFFSET>(this->modelId_, this->pos_[0], this->pos_[1], this->pos_[2], &newObjectHandle);
	const auto newObject = CPools::GetObject(newObjectHandle);

	CStreaming::SetMissionDoesntRequireModel(this->modelId_);

	return newObject;
}

Object::Object(const char* name, const CVector& pos): ObjectiveDependent(nullptr),
													  uuid_(boost::uuids::random_generator()()),
													  rotate{{0,0,0}, 1},
                                                      modelId_(325) {
	strlcpy(this->name_, name, sizeof this->name_);
	this->pos_[0] = pos.x;
	this->pos_[1] = pos.y;
	this->pos_[2] = pos.z;
}

Object::Object(const Object& other): ObjectiveDependent{other},
                                     INameable{other},
                                     IPositionable{other},
                                     IUuidable{other},
                                     uuid_{ boost::uuids::random_generator()() },
                                     rotate{other.rotate},
                                     scale_{other.scale_},
                                     modelId_{other.modelId_} {
	strlcat(name_, other.name_, sizeof name_);
	strlcat(name_, "C", sizeof name_);
	memcpy(this->pos_, other.pos_, sizeof this->pos_);
}

Object& Object::operator=(const Object& other) {
	if (this == &other)
		return *this;
	ObjectiveDependent::operator =(other);
	INameable::operator =(other);
	IPositionable::operator =(other);
	IUuidable::operator =(other);
	uuid_ = other.uuid_;
	editorObject_ = other.editorObject_;
	projectObject_ = other.projectObject_;
	rotate = other.rotate;
	scale_ = other.scale_;
	modelId_ = other.modelId_;
	strlcat(name_, other.name_, sizeof name_);
	memcpy(this->pos_, other.pos_, sizeof this->pos_);
	return *this;
}

Object::~Object() {
	this->deleteEditorObject();
	this->deleteProjectEntity();
}

boost::uuids::uuid& Object::getUuid() {
	return uuid_;
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
	return scale_;
}

int& Object::getModelId() {
	return modelId_;
}

void Object::updateLocation() {
	CQuaternion rw;
	rw.real = this->rotate.real;
	rw.imag.x = this->rotate.imag.z;
	rw.imag.y = this->rotate.imag.x;
	rw.imag.z = -this->rotate.imag.y;
	CMatrix newMatrix;
	newMatrix.SetRotate(rw);
	newMatrix.SetTranslateOnly(this->pos_[0], this->pos_[1], this->pos_[2]);
	newMatrix.right *= this->scale_[2];
	newMatrix.up *= this->scale_[0];
	newMatrix.at *= this->scale_[1];

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

char* Object::getName() {
	return this->name_;
}

float* Object::getPosition() {
	return pos_;
}

void Object::spawnEditorObject() {
	if (editorObject_.has_value())
		this->deleteEditorObject();

	this->editorObject_ = spawnObject();

	Command<Commands::FREEZE_OBJECT_POSITION>(this->editorObject_.value(), 1);
	Command<Commands::SET_OBJECT_DYNAMIC>(this->editorObject_.value(), 0);

	updateLocation();
}

extern bool restart;
void Object::deleteEditorObject() {
	if (this->editorObject_.has_value() && !restart) {
		int objectRef = CPools::GetObjectRef(this->editorObject_.value());
		Command<Commands::DELETE_OBJECT>(objectRef);
		this->editorObject_ = std::nullopt;
	}
}

void Object::spawnProjectEntity() {
	if (projectObject_.has_value())
		this->deleteProjectEntity();

	this->projectObject_ = spawnObject();

	updateLocation();
}

void Object::deleteProjectEntity() {
	if (this->projectObject_.has_value() && !restart) {
		int objectRef = CPools::GetObjectRef(this->projectObject_.value());
		Command<Commands::DELETE_OBJECT>(objectRef);
		this->projectObject_ = std::nullopt;
	}
}
