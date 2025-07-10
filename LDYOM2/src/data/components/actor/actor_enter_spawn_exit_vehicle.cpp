#include "actor_enter_spawn_exit_vehicle.h"
#include "components/actor.h"
#include "components/vehicle.h"
#include "entity.h"
#include "extensions/ScriptCommands.h"
#include "utils_entities_selections.h"
#include <entities_manager.h>
#include <in_game/actor_paths_editing.h>
#include <project_player.h>
#include <scenes_manager.h>
#include <window_manager.h>

void components::ActorEnterSpawnExitVehicle::sol_lua_register(sol::state_view lua_state) {
	auto ut = lua_state.new_usertype<ActorEnterSpawnExitVehicle>("ActorEnterSpawnExitVehicleComponent");
	SOL_LUA_FOR_EACH(SOL_LUA_BIND_MEMBER_ACTION, ut, components::ActorEnterSpawnExitVehicle, cast, nearestVehicle,
	                 vehicleId, seatIndex, taskType);
}

components::ActorEnterSpawnExitVehicle::ActorEnterSpawnExitVehicle()
	: Component(TYPE) {}

void components::ActorEnterSpawnExitVehicle::run() {
	const auto actor = Actor::cast(this->entity->getComponent(Actor::TYPE));
	if (!actor || !actor->ped) {
		return;
	}

	auto vehicle = findTargetVehicle(actor);
	if (!vehicle || !vehicle->handle) {
		return;
	}

	applyVehicleTask(actor, vehicle);
}

std::shared_ptr<components::Vehicle>
components::ActorEnterSpawnExitVehicle::findTargetVehicle(std::shared_ptr<components::Actor> actor) {
	return nearestVehicle ? findNearestVehicle(actor) : findVehicleById();
}

std::shared_ptr<components::Vehicle>
components::ActorEnterSpawnExitVehicle::findNearestVehicle(std::shared_ptr<components::Actor> actor) {
	auto entities = ProjectPlayer::instance().getEntities();
	std::vector<Entity*> vehicles;
	std::ranges::copy_if(entities, std::back_inserter(vehicles),
	                     [](Entity* entity) { return entity->hasComponent(Vehicle::TYPE); });

	if (vehicles.empty()) {
		return nullptr;
	}

	if (vehicles.size() == 1) {
		return Vehicle::cast(vehicles.front()->getComponent(Vehicle::TYPE));
	}

	auto nearestVehicleIt = std::ranges::min_element(vehicles, [actor](Entity* a, Entity* b) {
		const auto aVehicle = Vehicle::cast(a->getComponent(Vehicle::TYPE));
		const auto bVehicle = Vehicle::cast(b->getComponent(Vehicle::TYPE));
		if (!aVehicle || !bVehicle) {
			return false;
		}
		return DistanceBetweenPoints(aVehicle->handle->GetPosition(), actor->ped->GetPosition()) <
		       DistanceBetweenPoints(bVehicle->handle->GetPosition(), actor->ped->GetPosition());
	});

	return nearestVehicleIt != vehicles.end() ? Vehicle::cast((*nearestVehicleIt)->getComponent(Vehicle::TYPE))
	                                          : nullptr;
}

std::shared_ptr<components::Vehicle> components::ActorEnterSpawnExitVehicle::findVehicleById() {
	auto vehicleUuid = uuids::uuid::from_string(vehicleId);
	auto entities = ProjectPlayer::instance().getEntities();
	auto vehicleIt = std::ranges::find_if(entities, [vehicleUuid](Entity* entity) {
		return entity->id == vehicleUuid && entity->hasComponent(Vehicle::TYPE);
	});

	return vehicleIt != entities.end() ? Vehicle::cast((*vehicleIt)->getComponent(Vehicle::TYPE)) : nullptr;
}

void components::ActorEnterSpawnExitVehicle::applyVehicleTask(std::shared_ptr<components::Actor> actor,
                                                              std::shared_ptr<components::Vehicle> vehicle) {
	const bool isDriver = (seatIndex == 0);
	const int passengerSeat = seatIndex - 1;

	switch (taskType) {
		case 0: // Enter
			if (isDriver) {
				plugin::Command<plugin::Commands::TASK_ENTER_CAR_AS_DRIVER>(actor->getPedRef(),
				                                                            vehicle->getVehicleRef(), 10000);
			} else {
				plugin::Command<plugin::Commands::TASK_ENTER_CAR_AS_PASSENGER>(
					actor->getPedRef(), vehicle->getVehicleRef(), 10000, passengerSeat);
			}
			break;
		case 1: // Spawn
			if (isDriver) {
				plugin::Command<plugin::Commands::TASK_WARP_CHAR_INTO_CAR_AS_DRIVER>(actor->getPedRef(),
				                                                                     vehicle->getVehicleRef());
			} else {
				plugin::Command<plugin::Commands::TASK_WARP_CHAR_INTO_CAR_AS_PASSENGER>(
					actor->getPedRef(), vehicle->getVehicleRef(), passengerSeat);
			}
			break;
		case 2: // Exit
			if (isDriver) {
				plugin::Command<plugin::Commands::TASK_WARP_CHAR_INTO_CAR_AS_DRIVER>(actor->getPedRef(),
				                                                                     vehicle->getVehicleRef());
			} else {
				plugin::Command<plugin::Commands::TASK_WARP_CHAR_INTO_CAR_AS_PASSENGER>(
					actor->getPedRef(), vehicle->getVehicleRef(), passengerSeat);
			}
			ProjectPlayer::instance().projectTasklist->add_task(
				[](ActorEnterSpawnExitVehicle* _this, std::shared_ptr<components::Actor> actor,
			       std::shared_ptr<components::Vehicle> vehicle) -> ktwait {
					if (actor->ped && vehicle->handle) {
						co_await 1000;
						plugin::Command<plugin::Commands::TASK_LEAVE_CAR>(actor->getPedRef(), vehicle->getVehicleRef());
					}
				},
				this, actor, vehicle);
			break;
		default: break;
	}
}

nlohmann::json components::ActorEnterSpawnExitVehicle::to_json() const {
	auto j = Component::to_json();
	j["nearestVehicle"] = nearestVehicle;
	j["vehicleId"] = vehicleId;
	j["seatIndex"] = seatIndex;
	j["taskType"] = taskType;
	return j;
}

void components::ActorEnterSpawnExitVehicle::from_json(const nlohmann::json& j) {
	Component::from_json(j);
	nearestVehicle = j.value("nearestVehicle", false);
	vehicleId = j.value("vehicleId", std::string{});
	seatIndex = j.value("seatIndex", 0);
	taskType = j.value("taskType", 0);
}

void components::ActorEnterSpawnExitVehicle::editorRender() {
	constexpr float labelWidth = 0.45f;
	constexpr int minSeatIndex = 0;
	constexpr int maxSeatIndex = 3;

	const auto availableWidth = ImGui::GetContentRegionAvail().x;

	// Nearest vehicle checkbox
	ImGui::Text(tr("nearest_vehicle").c_str());
	ImGui::SameLine(availableWidth * labelWidth);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::Checkbox("##nearestVehicle", &nearestVehicle);

	// Vehicle ID selection (only when not using nearest vehicle)
	if (!nearestVehicle) {
		renderEntityByComponentSelection<components::Vehicle>(vehicleId, availableWidth, labelWidth);
	}

	// Seat index slider
	ImGui::Text(tr("seat_index").c_str());
	ImGui::SameLine(availableWidth * labelWidth);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::SliderInt("##seatIndex", &seatIndex, minSeatIndex, maxSeatIndex,
	                 tr(fmt::format("seat_indexes.{}", seatIndex)).c_str());

	// Task type selection
	ImGui::Text(tr("task_type").c_str());
	ImGui::SameLine(availableWidth * labelWidth);
	ImGui::SetNextItemWidth(-1.f);
	if (ImGui::BeginCombo("##taskType", tr(fmt::format("task_types.{}", taskType)).c_str())) {
		for (int i = 0; i < 3; ++i) {
			const bool isSelected = (i == taskType);
			if (ImGui::Selectable(tr(fmt::format("task_types.{}", i)).c_str(), isSelected)) {
				taskType = i;
			}
			if (isSelected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
}

void components::ActorEnterSpawnExitVehicle::onStart() {
	Component::onStart();
	if (IS_PLAYING) {
		const auto actor = Actor::cast(this->entity->getComponent(Actor::TYPE));
		if (actor) {
			this->m_pedSpawnedConnection = std::make_optional(actor->onSpawned.connect([this]() {
				ProjectPlayer::instance().projectTasklist->add_task(
					[this](ActorEnterSpawnExitVehicle* _this) -> ktwait {
						_this->run();
						co_return;
					},
					this);
			}));
		}
	}
}

void components::ActorEnterSpawnExitVehicle::onUpdate(float deltaTime) { Component::onUpdate(deltaTime); }

void components::ActorEnterSpawnExitVehicle::onReset() {
	Component::onReset();
	this->m_pedSpawnedConnection.reset();
}

Dependencies components::ActorEnterSpawnExitVehicle::getDependencies() { return Dependencies{{Actor::TYPE}, true}; }