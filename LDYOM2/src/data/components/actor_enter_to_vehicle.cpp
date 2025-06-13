#include "actor_enter_to_vehicle.h"
#include "actor.h"
#include "entity.h"
#include "extensions/ScriptCommands.h"
#include "vehicle.h"
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
	if (actor && actor->ped) {
	}
}

nlohmann::json components::ActorEnterToVehicle::to_json() const {
	auto j = this->Component::to_json();
	j["nearestVehicle"] = nearestVehicle;
	j["vehicleId"] = vehicleId;
	j["seatIndex"] = seatIndex;
	j["teleportToVehicle"] = teleportToVehicle;
	return j;
}

void components::ActorEnterToVehicle::from_json(const nlohmann::json& j) {
	this->Component::from_json(j);
	j.at("nearestVehicle").get_to(nearestVehicle);
	j.at("vehicleId").get_to(vehicleId);
	j.at("seatIndex").get_to(seatIndex);
	j.at("teleportToVehicle").get_to(teleportToVehicle);
}

void components::ActorEnterToVehicle::editorRender() {
	const auto availableWidth = ImGui::GetContentRegionAvail().x;

	ImGui::Text(tr("nearest_vehicle").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::Checkbox("##nearestVehicle", &nearestVehicle);

	ImGui::Text(tr("vehicle_id").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	const auto vehicles = EntitiesManager::instance().getEntitiesWithComponent(Vehicle::TYPE);
	const auto vehicleUuid = uuids::uuid::from_string(vehicleId);
	const auto currentVehicle =
		std::ranges::find_if(vehicles, [vehicleUuid](Entity* entity) { return entity->id == vehicleUuid; });
	if (ImGui::BeginCombo("##vehicleId", currentVehicle != vehicles.end() ? (*currentVehicle)->name.c_str() : "")) {
		for (const auto& vehicle : vehicles) {
			if (ImGui::Selectable(vehicle->name.c_str(), vehicle->id == vehicleUuid)) {
				this->vehicleId = uuids::to_string(vehicle->id);
			}
		}
		ImGui::EndCombo();
	}

	ImGui::Text(tr("seat_index").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::SliderInt("##seatIndex", &seatIndex, 0, 3, tr(fmt::format("seat_indexes.{}", seatIndex)).c_str());

	ImGui::Text(tr("teleport_to_vehicle").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::Checkbox("##teleportToVehicle", &teleportToVehicle);
}

void components::ActorEnterToVehicle::onStart() {
	Component::onStart();
	if (IS_PLAYING) {
		const auto actor = Actor::cast(this->entity->getComponent(Actor::TYPE));
		if (actor) {
			if (actor->ped) {
				run();
			}
			this->m_pedSpawnedConnection = std::make_optional(actor->onSpawned.connect([this]() { run(); }));
		}
	}
}

void components::ActorEnterToVehicle::onUpdate(float deltaTime) { Component::onUpdate(deltaTime); }

void components::ActorEnterToVehicle::onReset() {
	Component::onReset();
	this->m_pedSpawnedConnection.reset();
}

Dependencies components::ActorEnterToVehicle::getDependencies() { return Dependencies{{Actor::TYPE}, true}; }