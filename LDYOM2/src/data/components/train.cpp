#define IMGUI_DEFINE_MATH_OPERATORS
#include "train.h"
#include "extensions/ScriptCommands.h"
#include <CStreaming.h>
#include <eModelID.h>
#include <entity.h>
#include <in_game/entity_orbit_camera.h>
#include <lua_define_type.h>
#include <project_player.h>
#include <scenes_manager.h>
#include <spdlog/spdlog.h>
#include <task_manager.h>
#include <window_manager.h>
#include <windows/entities.h>
#include <windows/entity_info_panel.h>

using namespace plugin;

void components::Train::sol_lua_register(sol::state_view lua_state) {
	sol_lua_register_enum_DirtyFlags(lua_state);
	auto ut = lua_state.new_usertype<Train>("TrainComponent");
	SOL_LUA_FOR_EACH(SOL_LUA_BIND_MEMBER_ACTION, ut, components::Train, cast, trainType, rotate, speed, cruiseSpeed,
	                 health, despawn, getTrainRef);
}

components::Train::Train()
	: Component(TYPE) {}

nlohmann::json components::Train::to_json() const {
	auto j = this->Component::to_json();
	j["trainType"] = trainType;
	j["rotate"] = rotate;
	j["speed"] = speed;
	j["cruiseSpeed"] = cruiseSpeed;
	j["health"] = health;
	return j;
}

void components::Train::from_json(const nlohmann::json& j) {
	this->Component::from_json(j);
	j.at("trainType").get_to(trainType);
	j.at("rotate").get_to(rotate);
	j.at("speed").get_to(speed);
	j.at("cruiseSpeed").get_to(cruiseSpeed);
	j.at("health").get_to(health);
}

void components::Train::editorRender() {
	const auto availableWidth = ImGui::GetContentRegionAvail().x;

	// Train Type selector
	ImGui::Text(tr("train_type").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);

	if (ImGui::BeginCombo("##trainType", tr(fmt::format("train_types.{}", trainType)).c_str())) {
		for (int i = 0; i < 16; ++i) {
			if (ImGui::Selectable(tr(fmt::format("train_types.{}", i)).c_str(), trainType == i)) {
				trainType = i;
				dirty |= DirtyFlags::Model;
			}
		}
		ImGui::EndCombo();
	}

	ImGui::Spacing();

	// Rotate checkbox
	ImGui::Text(tr("rotate_180").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	if (ImGui::Checkbox("##rotate", &rotate)) {
		dirty |= DirtyFlags::Model;
	}

	ImGui::Spacing();

	// Speed
	ImGui::Text(tr("speed").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	if (ImGui::DragFloat("##speed", &speed, 0.1f, -50.0f, 50.0f, "%.1f")) {
		dirty |= DirtyFlags::Speed;
	}

	ImGui::Spacing();

	// Cruise Speed
	ImGui::Text(tr("cruise_speed").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	if (ImGui::DragFloat("##cruiseSpeed", &cruiseSpeed, 0.1f, -50.0f, 50.0f, "%.1f")) {
		dirty |= DirtyFlags::Speed;
	}

	ImGui::Spacing();

	// Health
	ImGui::Text(tr("health").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::DragFloat("##health", &health, 10.0f, 0.0f, 10000.0f, "%.0f");

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
}

void components::Train::updatePosition() {
	if (this->handle) {
		const CVector position{this->entity->position[0], this->entity->position[1], this->entity->position[2]};
		this->handle->SetPosn(position);
	}
}

void components::Train::spawn() {
	this->despawn();

	// Request models based on train type
	switch (this->trainType) {
		case 0:
		case 3:
		case 6:
		case 10:
		case 12:
		case 13:
			// Freight trains
			CStreaming::RequestModel(MODEL_FREIGHT, GAME_REQUIRED);
			CStreaming::RequestModel(MODEL_FREIFLAT, GAME_REQUIRED);
			break;
		case 1:
		case 2:
		case 4:
		case 5:
		case 7:
		case 11:
			// Streak trains
			CStreaming::RequestModel(MODEL_STREAK, GAME_REQUIRED);
			CStreaming::RequestModel(MODEL_STREAKC, GAME_REQUIRED);
			break;
		case 8:
		case 9:
		case 14:
			// Trams
			CStreaming::RequestModel(MODEL_TRAM, GAME_REQUIRED);
			break;
		case 15:
			// Single streak car
			CStreaming::RequestModel(MODEL_STREAK, GAME_REQUIRED);
			break;
		default: break;
	}

	CStreaming::LoadAllRequestedModels(false);

	int trainHandle;
	Command<Commands::CREATE_MISSION_TRAIN>(this->trainType, this->entity->position[0], this->entity->position[1],
	                                        this->entity->position[2], this->rotate, &trainHandle);

	auto* train = static_cast<CTrain*>(CPools::GetVehicle(trainHandle));
	if (!train) {
		spdlog::error("Failed to spawn train");
		CStreaming::RemoveAllUnusedModels();
		return;
	}

	this->handle = std::shared_ptr<CTrain>(train, [](CTrain* t) {
		if (t && CPools::ms_pVehiclePool && CPools::ms_pVehiclePool->IsObjectValid(t)) {
			const int ref = CPools::GetVehicleRef(t);
			Command<Commands::DELETE_MISSION_TRAIN>(ref);
		}
	});

	train->m_bStreamingDontDelete = 1;
	train->m_fHealth = this->health;
	Command<Commands::DONT_REMOVE_CAR>(trainHandle);

	// Set speeds
	if (IS_PLAYING) {
		train->m_nVehicleFlags.bEngineOn = 1;
		Command<Commands::SET_TRAIN_SPEED>(trainHandle, this->speed);
		Command<Commands::SET_TRAIN_CRUISE_SPEED>(trainHandle, this->cruiseSpeed);
	} else {
		// Editor mode
		train->m_nStatus = STATUS_ABANDONED;
		train->m_nVehicleFlags.bEngineOn = 0;
		train->m_eDoorLock = DOORLOCK_LOCKED;
		Command<Commands::FREEZE_CAR_POSITION>(static_cast<CVehicle*>(train), true);
		train->m_nVehicleFlags.bCanBeDamaged = false;
	}

	// Update entity position from spawned train
	const CVector trainPosition = train->GetPosition();
	this->entity->position = {trainPosition.x, trainPosition.y, trainPosition.z};

	CStreaming::RemoveAllUnusedModels();

	this->onSpawned();
}

void components::Train::despawn() {
	this->onSpawnedConnection.reset();

	if (this->handle) {
		this->handle.reset();
		this->onDespawned();
	}
}

void components::Train::onStart() {
	Component::onStart();

	// Set up transform callbacks for position synchronization
	this->entity->setGetTransformCallbacks(
		[this]() -> std::array<float, 3> {
			if (this->handle) {
				auto& position = this->handle->GetPosition();
				return {position.x, position.y, position.z};
			}
			return this->entity->position;
		},
		[this]() -> CQuaternion {
			// Trains don't support rotation queries via API
			return {};
		});

	this->entity->setSetTransformCallbacks(
		[this](const std::array<float, 3>& position) { this->dirty |= DirtyFlags::Model; },
		[this](const CQuaternion& rotation) {
			// Trains don't support rotation changes via API
		});

	this->spawn();
}

void components::Train::onUpdate(float deltaTime) {
	Component::onUpdate(deltaTime);

	if (this->dirty & DirtyFlags::Model) {
		this->spawn();
	} else if (this->dirty & DirtyFlags::Speed && this->handle && IS_PLAYING) {
		const int trainRef = this->getTrainRef();
		if (trainRef != -1) {
			Command<Commands::SET_TRAIN_SPEED>(trainRef, this->speed);
			Command<Commands::SET_TRAIN_CRUISE_SPEED>(trainRef, this->cruiseSpeed);
		}
	}

	this->dirty = DirtyFlags::None;
}

void components::Train::onReset() {
	Component::onReset();
	this->despawn();
}
