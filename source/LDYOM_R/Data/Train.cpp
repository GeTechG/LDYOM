#include "Train.h"

#include <CPools.h>
#include <CStreaming.h>
#include <boost/uuid/random_generator.hpp>
#include <extensions/ScriptCommands.h>

#include "LuaEngine.h"
#include "ProjectPlayerService.h"
#include "strUtils.h"
#include "easylogging/easylogging++.h"
#include "Scene.h"

using namespace plugin;

CTrain* Train::spawnTrain() {
	switch (this->trainType_) {
	case 0:
	case 3:
	case 6:
	case 10:
	case 12:
	case 13:
		CStreaming::RequestModel(MODEL_FREIGHT, GAME_REQUIRED);
		CStreaming::RequestModel(MODEL_FREIFLAT, GAME_REQUIRED);
		break;
	case 1:
	case 2:
	case 4:
	case 5:
	case 7:
	case 11:
		CStreaming::RequestModel(MODEL_STREAK, GAME_REQUIRED);
		CStreaming::RequestModel(MODEL_STREAKC, GAME_REQUIRED);
		break;
	case 8:
	case 9:
	case 14:
		CStreaming::RequestModel(MODEL_TRAM, GAME_REQUIRED);
		break;
	case 15:
		CStreaming::RequestModel(MODEL_STREAK, GAME_REQUIRED);
		break;
	default: 
		break;
	}
	CStreaming::LoadAllRequestedModels(false);

	int handleTrain;
	Command<Commands::CREATE_MISSION_TRAIN>(this->trainType_, this->pos_[0], this->pos_[1], this->pos_[2], this->rotate_, &handleTrain);
	const auto train = static_cast<CTrain*>(CPools::GetVehicle(handleTrain));

	train->m_bStreamingDontDelete = 1;
	Command<Commands::DONT_REMOVE_CAR>(handleTrain);
	
	CStreaming::RemoveAllUnusedModels();

	return train;
}

Train::Train(const char* name, const CVector& pos): ObjectiveDependent(nullptr),
													uuid_(boost::uuids::random_generator()()) {
	strlcpy(this->name_, name, sizeof this->name_);
	this->pos_[0] = pos.x;
	this->pos_[1] = pos.y;
	this->pos_[2] = pos.z;
}

Train::Train(const Train& other): ObjectiveDependent{other},
                                  INameable{other},
                                  IPositionable{other},
                                  IUuidable{other},
                                  uuid_{ boost::uuids::random_generator()() },
                                  rotate_{other.rotate_},
                                  health_{other.health_},
                                  trainType_{other.trainType_},
                                  speed_{other.speed_},
                                  cruiseSpeed_{other.cruiseSpeed_} {
	strlcat(name_, other.name_, sizeof name_);
	strlcat(name_, "C", sizeof name_);
	memcpy(pos_, other.pos_, sizeof pos_);
}

Train& Train::operator=(const Train& other) {
	if (this == &other)
		return *this;
	ObjectiveDependent::operator =(other);
	INameable::operator =(other);
	IPositionable::operator =(other);
	IUuidable::operator =(other);
	uuid_ = other.uuid_;
	editorTrain_ = other.editorTrain_;
	projectTrain_ = other.projectTrain_;
	rotate_ = other.rotate_;
	health_ = other.health_;
	trainType_ = other.trainType_;
	speed_ = other.speed_;
	cruiseSpeed_ = other.cruiseSpeed_;
	strlcat(name_, other.name_, sizeof name_);
	memcpy(pos_, other.pos_, sizeof pos_);
	return *this;
}

Train::~Train() {
	this->deleteEditorTrain();
	this->deleteProjectEntity();
}

boost::uuids::uuid& Train::getUuid() {
	return this->uuid_;
}

std::optional<CTrain*>& Train::getEditorTrain() {
	return editorTrain_;
}

std::optional<CTrain*>& Train::getProjectTrain() {
	return projectTrain_;
}

bool& Train::isRotate() {
	return rotate_;
}

int& Train::getHealth() {
	return health_;
}

int& Train::getTrainType() {
	return trainType_;
}

float& Train::getSpeed() {
	return speed_;
}

float& Train::getCruiseSpeed() {
	return cruiseSpeed_;
}

void Train::updateLocation() {
	if (this->editorTrain_.has_value()) {
		this->spawnEditorTrain();
	}
	if (this->projectTrain_.has_value()) {
		this->spawnProjectEntity();
	}
}

char* Train::getName() {
	return this->name_;
}

float* Train::getPosition() {
	return this->pos_;
}

void Train::spawnEditorTrain() {
	if (editorTrain_.has_value())
		this->deleteEditorTrain();

	this->editorTrain_ = spawnTrain();
	this->editorTrain_.value()->m_nStatus = STATUS_ABANDONED;
	this->editorTrain_.value()->m_nVehicleFlags.bEngineOn = 0;
	this->editorTrain_.value()->m_eDoorLock = DOORLOCK_LOCKED;
	Command<Commands::FREEZE_CAR_POSITION>(dynamic_cast<CVehicle*>(this->editorTrain_.value()), true);
	this->editorTrain_.value()->m_nVehicleFlags.bCanBeDamaged = false;

	const CVector trainPosition = this->editorTrain_.value()->GetPosition();
	this->pos_[0] = trainPosition.x;
	this->pos_[1] = trainPosition.y;
	this->pos_[2] = trainPosition.z;
}

extern bool restart;
void Train::deleteEditorTrain() {
	if (this->editorTrain_.has_value() && !restart) {
		int vehicleRef = CPools::GetVehicleRef(this->editorTrain_.value());
		Command<Commands::DELETE_MISSION_TRAIN>(vehicleRef);
		this->editorTrain_ = std::nullopt;
	}
}

void Train::spawnProjectEntity() {
	if (projectTrain_.has_value())
		this->deleteProjectEntity();

	this->projectTrain_ = spawnTrain();

	auto scene = ProjectPlayerService::getInstance().getCurrentScene();
	auto tasklist = ProjectPlayerService::getInstance().getSceneTasklist();

	if (scene.has_value() && tasklist != nullptr) {
		const auto onTrainSpawn = LuaEngine::getInstance().getLuaState()["global_data"]["signals"]["onTrainSpawn"].get_or_create<sol::table>();
		for (auto [_, func] : onTrainSpawn) {
			if (const auto result = func.as<sol::function>()(scene.value(), tasklist, this->uuid_); !result.valid()) {
				const sol::error err = result;
				CLOG(ERROR, "lua") << err.what();
			}
		}
	}
}

void Train::deleteProjectEntity() {
	if (this->projectTrain_.has_value() && !restart) {
		int vehicleRef = CPools::GetVehicleRef(this->projectTrain_.value());
		Command<Commands::DELETE_MISSION_TRAIN>(vehicleRef);
		this->projectTrain_ = std::nullopt;

		auto scene = ProjectPlayerService::getInstance().getCurrentScene();
		auto tasklist = ProjectPlayerService::getInstance().getSceneTasklist();

		if (scene.has_value() && tasklist != nullptr) {
			const auto onTrainDelete = LuaEngine::getInstance().getLuaState()["global_data"]["signals"]["onTrainDelete"].get_or_create<sol::table>();
			for (auto [_, func] : onTrainDelete) {
				if (const auto result = func.as<sol::function>()(scene.value(), tasklist, this->uuid_); !result.valid()) {
					const sol::error err = result;
					CLOG(ERROR, "lua") << err.what();
				}
			}
		}
	}
}
