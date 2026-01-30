#include "damage.h"
#include "components/vehicle.h"
#include "entity.h"
#include <CAutomobile.h>
#include <CPools.h>
#include <extensions/ScriptCommands.h>
#include <fa_icons.h>
#include <fmt/format.h>
#include <plugin.h>
#include <spdlog/spdlog.h>

using namespace plugin;

void components::VehicleDamage::sol_lua_register(sol::state_view lua_state) {
	sol_lua_register_enum_DirtyFlags(lua_state);
	auto ut = lua_state.new_usertype<VehicleDamage>("DamageComponent");
	SOL_LUA_FOR_EACH(SOL_LUA_BIND_MEMBER_ACTION, ut, components::VehicleDamage, cast, dirty, damageComponents,
	                 applyDamage);
}

components::VehicleDamage::VehicleDamage()
	: Component(TYPE) {}

void components::VehicleDamage::applyDamageToVehicle() {
	auto vehicle = Vehicle::cast(this->entity->getComponent(Vehicle::TYPE));
	if (!vehicle || !vehicle->handle) {
		return;
	}

	auto* vehiclePtr = vehicle->handle.get();
	if (!vehiclePtr) {
		return;
	}

	// Check if this is an automobile (most vehicles)
	if (vehiclePtr->m_nVehicleClass != VEHICLE_AUTOMOBILE && vehiclePtr->m_nVehicleClass != VEHICLE_BIKE &&
	    vehiclePtr->m_nVehicleClass != VEHICLE_QUAD) {
		return;
	}

	auto* automobile = static_cast<CAutomobile*>(vehiclePtr);

	// Apply damage to each component (17 components)
	for (int i = 0; i < 17; ++i) {
		if (this->damageComponents[i] < 199.f) {
			// Apply progressive damage
			automobile->m_damageManager.ApplyDamage(automobile, static_cast<tComponent>(i + 1), damageComponents[i],
			                                        1.f);
		} else {
			// Complete destruction
			if (i + 1 <= 4) {
				// Burst tire (components 1-4: wheels)
				const int vehicleRef = CPools::GetVehicleRef(vehiclePtr);
				Command<Commands::BURST_CAR_TYRE>(vehicleRef, i);
			} else if (i + 1 <= 10) {
				// Pop door (components 5-10: doors)
				const int door = i + 1 - 5;
				const int vehicleRef = CPools::GetVehicleRef(vehiclePtr);
				Command<Commands::POP_CAR_DOOR>(vehicleRef, door, false);
			} else {
				// Pop panel (components 11-17: panels)
				const int panel = i + 1 - 11;
				const int vehicleRef = CPools::GetVehicleRef(vehiclePtr);
				Command<Commands::POP_CAR_PANEL>(vehicleRef, panel, false);
			}
		}
	}
}

void components::VehicleDamage::applyDamage() { this->dirty |= ApplyDamage; }

nlohmann::json components::VehicleDamage::to_json() const {
	auto j = this->Component::to_json();
	j["damageComponents"] = damageComponents;
	return j;
}

void components::VehicleDamage::from_json(const nlohmann::json& j) {
	this->Component::from_json(j);
	if (j.contains("damageComponents")) {
		j.at("damageComponents").get_to(damageComponents);
	}
}

void components::VehicleDamage::editorRender() {
	const auto availableWidth = ImGui::GetContentRegionAvail().x;

	// Wheels section
	if (ImGui::TreeNodeEx(tr("wheels").c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
		for (int i = 0; i < 4; ++i) {
			ImGui::PushID(i);
			ImGui::Text("%s", tr(fmt::format("wheel_names.{}", i)).c_str());
			ImGui::SameLine(availableWidth * 0.35f);
			ImGui::SetNextItemWidth(-1.f);
			if (ImGui::SliderFloat("##wheel", &damageComponents[i], 0.0f, 250.0f, "%.1f")) {
				dirty |= ApplyDamage;
			}
			ImGui::PopID();
		}
		ImGui::TreePop();
	}

	ImGui::Spacing();

	// Doors section
	if (ImGui::TreeNodeEx(tr("doors").c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
		for (int i = 0; i < 6; ++i) {
			ImGui::PushID(100 + i);
			ImGui::Text("%s", tr(fmt::format("door_names.{}", i)).c_str());
			ImGui::SameLine(availableWidth * 0.35f);
			ImGui::SetNextItemWidth(-1.f);
			if (ImGui::SliderFloat("##door", &damageComponents[4 + i], 0.0f, 250.0f, "%.1f")) {
				dirty |= ApplyDamage;
			}
			ImGui::PopID();
		}
		ImGui::TreePop();
	}

	ImGui::Spacing();

	// Panels section
	if (ImGui::TreeNodeEx(tr("panels").c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
		for (int i = 0; i < 7; ++i) {
			ImGui::PushID(200 + i);
			ImGui::Text("%s", tr(fmt::format("panel_names.{}", i)).c_str());
			ImGui::SameLine(availableWidth * 0.35f);
			ImGui::SetNextItemWidth(-1.f);
			if (ImGui::SliderFloat("##panel", &damageComponents[10 + i], 0.0f, 250.0f, "%.1f")) {
				dirty |= ApplyDamage;
			}
			ImGui::PopID();
		}
		ImGui::TreePop();
	}

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	// Helper buttons
	if (ImGui::Button(fmt::format("{} {}", ICON_FA_ERASER, tr("reset_all")).c_str(), ImVec2(-1, 0))) {
		damageComponents.fill(0.0f);
		dirty |= ApplyDamage;
	}

	if (ImGui::Button(fmt::format("{} {}", ICON_FA_EXPLOSION, tr("destroy_all")).c_str(), ImVec2(-1, 0))) {
		damageComponents.fill(250.0f);
		dirty |= ApplyDamage;
	}
}

void components::VehicleDamage::onStart() {
	Component::onStart();
	auto vehicle = Vehicle::cast(this->entity->getComponent(Vehicle::TYPE));
	if (vehicle) {
		if (vehicle->handle) {
			applyDamageToVehicle();
		}
		this->m_vehicleSpawnedConnection =
			std::make_optional(vehicle->onSpawned.connect([this]() { applyDamageToVehicle(); }));
	}
}

void components::VehicleDamage::onUpdate(float deltaTime) {
	Component::onUpdate(deltaTime);
	if (this->dirty & ApplyDamage) {
		auto vehicle = Vehicle::cast(this->entity->getComponent(Vehicle::TYPE));
		if (vehicle) {
			vehicle->despawn();
			vehicle->spawn();
		}
	}
	this->dirty = None;
}

void components::VehicleDamage::onReset() {
	Component::onReset();
	this->m_vehicleSpawnedConnection.reset();
}

Dependencies components::VehicleDamage::getDependencies() { return {{Vehicle::TYPE}, true}; }
