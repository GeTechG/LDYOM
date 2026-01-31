#include "appearance.h"
#include "components/vehicle.h"
#include "entity.h"
#include <CAutomobile.h>
#include <CPools.h>
#include <extensions/ScriptCommands.h>
#include <fa_icons.h>
#include <fmt/format.h>
#include <plugin.h>
#include <spdlog/spdlog.h>
#include <utils/vehicle.h>

using namespace plugin;

// Door indices for GTA SA
// 0 = bonnet (hood), 1 = boot (trunk),
// 2 = front_left, 3 = front_right, 4 = rear_left, 5 = rear_right
static const char* DOOR_NAMES[] = {"bonnet",           "boot",           "front_left_door",
                                   "front_right_door", "rear_left_door", "rear_right_door"};

void components::VehicleAppearance::sol_lua_register(sol::state_view lua_state) {
	sol_lua_register_enum_DirtyFlags(lua_state);
	auto ut = lua_state.new_usertype<VehicleAppearance>("AppearanceComponent");
	SOL_LUA_FOR_EACH(SOL_LUA_BIND_MEMBER_ACTION, ut, components::VehicleAppearance, cast, dirty, extraPartsAngle,
	                 isLightsOn, openDoorsRation, dirtyLevel, heavy, applyAppearance);
}

components::VehicleAppearance::VehicleAppearance()
	: Component(TYPE) {}

void components::VehicleAppearance::applyAppearanceToVehicle() {
	auto vehicle = Vehicle::cast(this->entity->getComponent(Vehicle::TYPE));
	if (!vehicle || !vehicle->handle) {
		return;
	}

	auto* vehiclePtr = vehicle->handle.get();
	if (!vehiclePtr) {
		return;
	}

	// Apply hydraulics/extra parts angle (for lowriders, etc.)
	if (vehiclePtr->m_nVehicleClass == VEHICLE_AUTOMOBILE || vehiclePtr->m_nVehicleClass == VEHICLE_BIKE ||
	    vehiclePtr->m_nVehicleClass == VEHICLE_QUAD) {
		auto* automobile = static_cast<CAutomobile*>(vehiclePtr);
		automobile->UpdateMovingCollision(this->extraPartsAngle);
	}

	// Apply lights state
	const int vehicleRef = CPools::GetVehicleRef(vehiclePtr);
	Command<Commands::FORCE_CAR_LIGHTS>(vehicleRef, this->isLightsOn ? 2 : 0);
	vehiclePtr->m_nVehicleFlags.bEngineOn = static_cast<unsigned char>(this->isLightsOn);

	// Apply door open ratios
	for (int i = 0; i < 6; ++i) {
		const int carNodeIndex = GetCarNodeIndexFromDoor(i);
		vehiclePtr->OpenDoor(nullptr, carNodeIndex, static_cast<eDoors>(i), this->openDoorsRation[i], false);
	}

	// Apply dirt level
	vehiclePtr->m_fDirtLevel = this->dirtyLevel;

	// Apply heavy flag
	Command<Commands::SET_CAR_HEAVY>(vehicleRef, this->heavy);
}

void components::VehicleAppearance::applyAppearance() { this->dirty |= ApplyAppearance; }

nlohmann::json components::VehicleAppearance::to_json() const {
	auto j = this->Component::to_json();
	j["extraPartsAngle"] = extraPartsAngle;
	j["isLightsOn"] = isLightsOn;
	j["openDoorsRation"] = openDoorsRation;
	j["dirtyLevel"] = dirtyLevel;
	j["heavy"] = heavy;
	return j;
}

void components::VehicleAppearance::from_json(const nlohmann::json& j) {
	this->Component::from_json(j);
	if (j.contains("extraPartsAngle")) {
		j.at("extraPartsAngle").get_to(extraPartsAngle);
	}
	if (j.contains("isLightsOn")) {
		j.at("isLightsOn").get_to(isLightsOn);
	}
	if (j.contains("openDoorsRation")) {
		j.at("openDoorsRation").get_to(openDoorsRation);
	}
	if (j.contains("dirtyLevel")) {
		j.at("dirtyLevel").get_to(dirtyLevel);
	}
	if (j.contains("heavy")) {
		j.at("heavy").get_to(heavy);
	}
}

void components::VehicleAppearance::editorRender() {
	const auto availableWidth = ImGui::GetContentRegionAvail().x;

	// Hydraulics/Extra parts angle
	ImGui::Text(tr("extra_parts_angle").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	if (ImGui::SliderFloat("##extra_parts_angle", &extraPartsAngle, 0.0f, 1.0f, "%.2f")) {
		dirty |= ApplyAppearance;
	}

	ImGui::Spacing();

	// Lights state
	ImGui::Text(tr("lights_on").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	if (ImGui::Checkbox("##lights_on", &isLightsOn)) {
		dirty |= ApplyAppearance;
	}

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	// Doors section
	if (ImGui::TreeNodeEx(tr("doors_open_ratio").c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
		for (int i = 0; i < 6; ++i) {
			ImGui::PushID(i);
			ImGui::Text("%s", tr(DOOR_NAMES[i]).c_str());
			ImGui::SameLine(availableWidth * 0.35f);
			ImGui::SetNextItemWidth(-1.f);
			if (ImGui::SliderFloat("##door", &openDoorsRation[i], 0.0f, 1.0f, "%.2f")) {
				dirty |= ApplyAppearance;
			}
			ImGui::PopID();
		}
		ImGui::TreePop();
	}

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	// Dirt level
	ImGui::Text(tr("dirt_level").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	if (ImGui::SliderFloat("##dirt_level", &dirtyLevel, 0.0f, 15.0f, "%.1f")) {
		dirty |= ApplyAppearance;
	}

	ImGui::Spacing();

	// Heavy flag
	ImGui::Text(tr("heavy_vehicle").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	if (ImGui::Checkbox("##heavy", &heavy)) {
		dirty |= ApplyAppearance;
	}

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	// Helper buttons
	if (ImGui::Button(fmt::format("{} {}", ICON_FA_ERASER, tr("reset_all")).c_str(), ImVec2(-1, 0))) {
		extraPartsAngle = 0.0f;
		isLightsOn = false;
		openDoorsRation.fill(0.0f);
		dirtyLevel = 0.0f;
		heavy = false;
		dirty |= ApplyAppearance;
	}

	if (ImGui::Button(fmt::format("{} {}", ICON_FA_LIGHTBULB, tr("lights_and_clean")).c_str(), ImVec2(-1, 0))) {
		isLightsOn = true;
		dirtyLevel = 0.0f;
		dirty |= ApplyAppearance;
	}

	if (ImGui::Button(fmt::format("{} {}", ICON_FA_DOOR_OPEN, tr("open_all_doors")).c_str(), ImVec2(-1, 0))) {
		openDoorsRation.fill(1.0f);
		dirty |= ApplyAppearance;
	}

	if (ImGui::Button(fmt::format("{} {}", ICON_FA_DOOR_CLOSED, tr("close_all_doors")).c_str(), ImVec2(-1, 0))) {
		openDoorsRation.fill(0.0f);
		dirty |= ApplyAppearance;
	}
}

void components::VehicleAppearance::onStart() {
	Component::onStart();
	auto vehicle = Vehicle::cast(this->entity->getComponent(Vehicle::TYPE));
	if (vehicle) {
		if (vehicle->handle) {
			applyAppearanceToVehicle();
		}
		this->m_vehicleSpawnedConnection =
			std::make_optional(vehicle->onSpawned.connect([this]() { applyAppearanceToVehicle(); }));
	}
}

void components::VehicleAppearance::onUpdate(float deltaTime) {
	Component::onUpdate(deltaTime);
	if (this->dirty & ApplyAppearance) {
		applyAppearanceToVehicle();
	}
	this->dirty = None;
}

void components::VehicleAppearance::onReset() {
	Component::onReset();
	this->m_vehicleSpawnedConnection.reset();
}

void components::VehicleAppearance::onDestroy() {
	const auto vehicle = Vehicle::cast(this->entity->getComponent(Vehicle::TYPE));
	if (vehicle) {
		vehicle->dirty |= Vehicle::DirtyFlags::Model;
	}
}

Dependencies components::VehicleAppearance::getDependencies() { return {{Vehicle::TYPE}, true}; }
