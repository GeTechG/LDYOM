#include "actor_enter_to_vehicle.h"
#include "components/actor.h"
#include "components/vehicle.h"
#include "entity.h"
#include "extensions/ScriptCommands.h"
#include <entities_manager.h>
#include <in_game/actor_paths_editing.h>
#include <project_player.h>
#include <scenes_manager.h>
#include <window_manager.h>

void components::ActorEnterToVehicle::sol_lua_register(sol::state_view lua_state) {
	auto ut = lua_state.new_usertype<ActorEnterToVehicle>("ActorEnterToVehicleComponent");
	SOL_LUA_FOR_EACH(SOL_LUA_BIND_MEMBER_ACTION, ut, components::ActorEnterToVehicle, cast, nearestVehicle, vehicleId,
	                 seatIndex, teleportToVehicle);
}

components::ActorEnterToVehicle::ActorEnterToVehicle()
	: Component(TYPE) {}

void components::ActorEnterToVehicle::run() {
	const auto actor = Actor::cast(this->entity->getComponent(Actor::TYPE));
	if (!actor || !actor->ped) {
		return;
	}

	auto vehicle = findTargetVehicle(actor);
	if (!vehicle || !vehicle->handle) {
		return;
	}

	enterVehicle(actor, vehicle);
}

std::shared_ptr<components::Vehicle>
components::ActorEnterToVehicle::findTargetVehicle(std::shared_ptr<components::Actor> actor) {
	return nearestVehicle ? findNearestVehicle(actor) : findVehicleById();
}

std::shared_ptr<components::Vehicle>
components::ActorEnterToVehicle::findNearestVehicle(std::shared_ptr<components::Actor> actor) {
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

std::shared_ptr<components::Vehicle> components::ActorEnterToVehicle::findVehicleById() {
	auto vehicleUuid = uuids::uuid::from_string(vehicleId);
	auto entities = ProjectPlayer::instance().getEntities();
	auto vehicleIt = std::ranges::find_if(entities, [vehicleUuid](Entity* entity) {
		return entity->id == vehicleUuid && entity->hasComponent(Vehicle::TYPE);
	});

	return vehicleIt != entities.end() ? Vehicle::cast((*vehicleIt)->getComponent(Vehicle::TYPE)) : nullptr;
}

void components::ActorEnterToVehicle::enterVehicle(std::shared_ptr<components::Actor> actor,
                                                   std::shared_ptr<components::Vehicle> vehicle) {
	const bool isDriver = (seatIndex == 0);
	const int passengerSeat = seatIndex - 1;

	if (teleportToVehicle) {
		if (isDriver) {
			plugin::Command<plugin::Commands::TASK_WARP_CHAR_INTO_CAR_AS_DRIVER>(actor->getPedRef(),
			                                                                     vehicle->getVehicleRef());
		} else {
			plugin::Command<plugin::Commands::TASK_WARP_CHAR_INTO_CAR_AS_PASSENGER>(
				actor->getPedRef(), vehicle->getVehicleRef(), passengerSeat);
		}
	} else {
		if (isDriver) {
			plugin::Command<plugin::Commands::TASK_ENTER_CAR_AS_DRIVER>(actor->getPedRef(), vehicle->getVehicleRef(),
			                                                            10000);
		} else {
			plugin::Command<plugin::Commands::TASK_ENTER_CAR_AS_PASSENGER>(actor->getPedRef(), vehicle->getVehicleRef(),
			                                                               10000, passengerSeat);
		}
	}
}

nlohmann::json components::ActorEnterToVehicle::to_json() const {
	auto j = Component::to_json();
	j["nearestVehicle"] = nearestVehicle;
	j["vehicleId"] = vehicleId;
	j["seatIndex"] = seatIndex;
	j["teleportToVehicle"] = teleportToVehicle;
	return j;
}

void components::ActorEnterToVehicle::from_json(const nlohmann::json& j) {
	Component::from_json(j);
	nearestVehicle = j.value("nearestVehicle", false);
	vehicleId = j.value("vehicleId", std::string{});
	seatIndex = j.value("seatIndex", 0);
	teleportToVehicle = j.value("teleportToVehicle", false);
}

void components::ActorEnterToVehicle::editorRender() {
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
		renderVehicleSelection(availableWidth, labelWidth);
	}

	// Seat index slider
	ImGui::Text(tr("seat_index").c_str());
	ImGui::SameLine(availableWidth * labelWidth);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::SliderInt("##seatIndex", &seatIndex, minSeatIndex, maxSeatIndex,
	                 tr(fmt::format("seat_indexes.{}", seatIndex)).c_str());

	// Teleport to vehicle checkbox
	ImGui::Text(tr("teleport_to_vehicle").c_str());
	ImGui::SameLine(availableWidth * labelWidth);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::Checkbox("##teleportToVehicle", &teleportToVehicle);
}

void components::ActorEnterToVehicle::renderVehicleSelection(float availableWidth, float labelWidth) {
	ImGui::Text(tr("vehicle_id").c_str());
	ImGui::SameLine(availableWidth * labelWidth);
	ImGui::SetNextItemWidth(-1.f);

	const auto vehicles = EntitiesManager::instance().getEntitiesWithComponent(Vehicle::TYPE);
	const auto vehicleUuid = uuids::uuid::from_string(vehicleId);
	const auto currentVehicle =
		std::ranges::find_if(vehicles, [vehicleUuid](Entity* entity) { return entity->id == vehicleUuid; });

	const char* previewText = (currentVehicle != vehicles.end()) ? (*currentVehicle)->name.c_str() : "";

	if (ImGui::BeginCombo("##vehicleId", previewText)) {
		for (size_t i = 0; i < vehicles.size(); ++i) {
			const auto& vehicle = vehicles[i];
			const bool isSelected = (vehicle->id == vehicleUuid);

			if (ImGui::Selectable(fmt::format("{}##{}", vehicle->name, i).c_str(), isSelected)) {
				vehicleId = to_string(vehicle->id);
			}
		}
		ImGui::EndCombo();
	}
}

void components::ActorEnterToVehicle::onStart() {
	Component::onStart();
	if (IS_PLAYING) {
		const auto actor = Actor::cast(this->entity->getComponent(Actor::TYPE));
		if (actor) {
			this->m_pedSpawnedConnection = std::make_optional(actor->onSpawned.connect([this]() {
				ProjectPlayer::instance().projectTasklist->add_task(
					[this](ActorEnterToVehicle* _this) -> ktwait {
						_this->run();
						co_return;
					},
					this);
			}));
		}
	}
}

void components::ActorEnterToVehicle::onUpdate(float deltaTime) { Component::onUpdate(deltaTime); }

void components::ActorEnterToVehicle::onReset() {
	Component::onReset();
	this->m_pedSpawnedConnection.reset();
}

Dependencies components::ActorEnterToVehicle::getDependencies() { return Dependencies{{Actor::TYPE}, true}; }