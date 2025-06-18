#define IMGUI_DEFINE_MATH_OPERATORS
#include "vehicle.h"
#include "extensions/ScriptCommands.h"
#include "objective_specific.h"
#include <CStreaming.h>
#include <corecrt_math_defines.h>
#include <entity.h>
#include <lua_define_type.h>
#include <popups/vehicle_selector.h>
#include <project_player.h>
#include <scenes_manager.h>
#include <string_utils.h>
#include <utils/imgui_configurate.h>

void components::Vehicle::sol_lua_register(sol::state_view lua_state) {
	sol_lua_register_enum_DirtyFlags(lua_state);
	auto ut = lua_state.new_usertype<Vehicle>("VehicleComponent");
	SOL_LUA_FOR_EACH(SOL_LUA_BIND_MEMBER_ACTION, ut, components::Vehicle, cast, initialDirection, model, primaryColorId,
	                 secondaryColorId, tertiaryColorId, quaternaryColorId, health, bulletproof, fireproof,
	                 explosionproof, collisionproof, meleeproof, tiresVulnerability, mustSurvive, locked, despawn,
	                 getVehicleRef);
}

components::Vehicle::Vehicle()
	: Component(TYPE) {
	this->initialDirection = FindPlayerPed()->GetHeading();
}

inline nlohmann::json components::Vehicle::to_json() const {
	auto j = this->Component::to_json();
	j["initialDirection"] = initialDirection;
	j["model"] = model;
	j["primaryColorId"] = primaryColorId;
	j["secondaryColorId"] = secondaryColorId;
	j["tertiaryColorId"] = tertiaryColorId;
	j["quaternaryColorId"] = quaternaryColorId;
	j["health"] = health;
	j["bulletproof"] = bulletproof;
	j["fireproof"] = fireproof;
	j["explosionproof"] = explosionproof;
	j["collisionproof"] = collisionproof;
	j["meleeproof"] = meleeproof;
	j["tiresVulnerability"] = tiresVulnerability;
	j["mustSurvive"] = mustSurvive;
	return j;
}

void components::Vehicle::from_json(const nlohmann::json& j) {
	this->Component::from_json(j);
	j.at("initialDirection").get_to(initialDirection);
	j.at("model").get_to(model);
	j.at("primaryColorId").get_to(primaryColorId);
	j.at("secondaryColorId").get_to(secondaryColorId);
	j.at("tertiaryColorId").get_to(tertiaryColorId);
	j.at("quaternaryColorId").get_to(quaternaryColorId);
	j.at("health").get_to(health);
	j.at("bulletproof").get_to(bulletproof);
	j.at("fireproof").get_to(fireproof);
	j.at("explosionproof").get_to(explosionproof);
	j.at("collisionproof").get_to(collisionproof);
	j.at("meleeproof").get_to(meleeproof);
	j.at("tiresVulnerability").get_to(tiresVulnerability);
	j.at("mustSurvive").get_to(mustSurvive);
}

struct VehicleColorData {
	unsigned char r;
	unsigned char g;
	unsigned char b;
	const char* name;
};

std::array<VehicleColorData, 127> vehicleColorsData = {{
	{0, 0, 0, "Black"},
	{245, 245, 245, "White"},
	{42, 119, 161, "Police Car Blue"},
	{132, 4, 16, "Cherry Red"},
	{38, 55, 57, "Midnight Blue"},
	{134, 68, 110, "Temple Curtain Purple"},
	{215, 142, 16, "Taxi Yellow"},
	{76, 117, 183, "Striking Blue"},
	{189, 190, 198, "Light Blue Grey"},
	{94, 112, 114, "Hoods"},
	{70, 89, 122, "Saxony Blue Poly"},
	{101, 106, 121, "Concord Blue Poly"},
	{93, 126, 141, "Jasper Green Poly"},
	{88, 89, 90, "Pewter Gray Poly"},
	{214, 218, 214, "Frost White"},
	{156, 161, 163, "Silver Stone Poly"},
	{51, 95, 63, "Medium Green"},
	{115, 14, 26, "Torino Red Pearl"},
	{123, 10, 42, "Formula Red"},
	{159, 157, 148, "Honey Beige Poly"},
	{59, 78, 120, "Mariner Blue"},
	{115, 46, 62, "Blaze Red"},
	{105, 30, 59, "Classic Red"},
	{150, 145, 140, "Winning Silver Poly"},
	{81, 84, 89, "Steel Gray Poly"},
	{63, 62, 69, "Shadow Silver Poly"},
	{165, 169, 167, "Silver Stone Poly"},
	{99, 92, 90, "Warm Grey Mica"},
	{61, 74, 104, "Harbor Blue Poly"},
	{151, 149, 146, "Porcelain Silver Poly"},
	{66, 31, 33, "Mellow Burgundy"},
	{95, 39, 43, "Graceful Red Mica"},
	{132, 148, 171, "Currant Blue Poly"},
	{118, 123, 124, "Gray Poly"},
	{100, 100, 100, "Arctic White"},
	{90, 87, 82, "Anthracite Gray Poly"},
	{37, 37, 39, "Black Poly"},
	{45, 58, 53, "Dark Green Poly"},
	{147, 163, 150, "Seafoam Poly"},
	{109, 122, 136, "Diamond Blue Poly"},
	{34, 25, 24, "Biston Brown Poly"},
	{111, 103, 95, "Desert Taupe Poly"},
	{124, 28, 42, "Garnet Red Poly"},
	{95, 10, 21, "Desert Red"},
	{25, 56, 38, "Dark Green"},
	{93, 27, 32, "Cabernet Red Poly"},
	{157, 152, 114, "Light Ivory"},
	{122, 117, 96, "Pueblo Beige"},
	{152, 149, 134, "Smoke Silver Poly"},
	{173, 176, 176, "Astra Silver Poly"},
	{132, 137, 136, "Ascot Gray"},
	{48, 79, 69, "Agate Green"},
	{77, 98, 104, "Petrol Blue Green Poly"},
	{22, 34, 72, "Surf Blue"},
	{39, 47, 75, "Nautical Blue Poly"},
	{125, 98, 86, "Woodrose Poly"},
	{158, 164, 171, "Crystal Blue Poly"},
	{156, 141, 113, "Bisque Frost Poly"},
	{109, 24, 34, "Currant Red Solid"},
	{78, 104, 129, "Light Crystal Blue Poly"},
	{156, 156, 152, "Light Titanium Poly"},
	{145, 115, 71, "Race Yellow Solid"},
	{102, 28, 38, "Bright Currant Red Poly"},
	{148, 157, 159, "Clear Crystal Blue Frost Poly"},
	{164, 167, 165, "Silver Poly"},
	{142, 140, 70, "Pastel Alabaster"},
	{52, 26, 30, "Mid Currant Red Poly"},
	{106, 122, 140, "Medium Regatta Blue Poly"},
	{170, 173, 142, "Oxford White Solid"},
	{171, 152, 143, "Alabaster Solid"},
	{133, 31, 46, "Elec. Currant Red Poly"},
	{111, 130, 151, "Spinnaker Blue Solid"},
	{88, 88, 83, "Darktitanium Poly"},
	{154, 167, 144, "Pastel Alabaster Solid"},
	{96, 26, 35, "Medium cabernet Solid"},
	{32, 32, 44, "Twilight Blue Poly"},
	{164, 160, 150, "Titanium Frost Poly"},
	{170, 157, 132, "Sandalwood Frost Poly"},
	{120, 34, 43, "Wild Strawberry Poly"},
	{14, 49, 109, "Ultra Blue Poly"},
	{114, 42, 63, "Vermilion Solid"},
	{123, 113, 94, "Medium Sandalwood Poly"},
	{116, 29, 40, "Medium Red Solid"},
	{30, 46, 50, "Deep Jewel Green"},
	{77, 50, 47, "Medium Woodrose Poly"},
	{124, 27, 68, "Vermillion Solid"},
	{46, 91, 32, "Green"},
	{57, 90, 131, "Bright Blue Poly"},
	{109, 40, 55, "Bright Red"},
	{167, 162, 143, "Light Champagne Poly"},
	{175, 177, 177, "Silver Poly"},
	{54, 65, 85, "Steel Blue Poly"},
	{109, 108, 110, "Medium Gray Poly"},
	{15, 106, 137, "Arctic Pearl"},
	{32, 75, 107, "Nassau Blue Poly"},
	{43, 62, 87, "Medium Sapphire Blue Poly"},
	{155, 159, 157, "Silver Poly"},
	{108, 132, 149, "Light Sapphire Blue Poly"},
	{84, 133, 148, "Malachite Poly"},
	{174, 155, 127, "Flax"},
	{64, 108, 143, "Medium Maui Blue Poly"},
	{31, 37, 59, "Darksapphire Blue Poly"},
	{171, 146, 118, "Copper Beige"},
	{19, 69, 115, "Bright Blue Poly"},
	{150, 129, 108, "Medium Flax"},
	{100, 104, 106, "Medium Gray Poly"},
	{16, 80, 130, "Bright Blue Poly"},
	{161, 153, 131, "Light Driftwood Poly"},
	{56, 86, 148, "Blue"},
	{82, 86, 97, "Steel Gray Poly"},
	{127, 105, 86, "Light Beechwood Poly"},
	{140, 146, 154, "Slate Gray"},
	{89, 110, 135, "Light Sapphire Blue Poly"},
	{71, 53, 50, "Dark Beech Wood Poly"},
	{68, 98, 79, "Flat Green"},
	{115, 10, 39, "Bright Red"},
	{34, 52, 87, "Medium Sapphire Blue Firemist"},
	{100, 13, 27, "Medium Garnet Red Poly"},
	{163, 173, 198, "White Diamond Pearl"},
	{105, 88, 83, "Darksable Poly"},
	{155, 139, 128, "Antelope Beige"},
	{98, 11, 28, "Brilliant Red Poly"},
	{91, 93, 94, "Gun Metal Poly"},
	{98, 68, 40, "Medium Beechwood Poly"},
	{115, 24, 39, "Brilliant Red Poly"},
	{27, 55, 109, "Bright Blue Poly"},
	{236, 106, 174, "Pink"},
}};

void components::Vehicle::editorRender() {
	const auto availableWidth = ImGui::GetContentRegionAvail().x;
	ImGui::Text("%s", tr("direction").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	if (ImGui::SliderAngle("##direction", &initialDirection, -180.0f, 180.0f, "%.0f°")) {
		dirty |= Direction;
	}

	ImGui::Text(_("model").c_str());
	ImGui::SameLine(availableWidth * 0.45f);

	ImGui::BeginGroup(); // Group for combo and button
	const float currentAvailableWidthInGroup = ImGui::GetContentRegionAvail().x;
	const float buttonWidth = ImGui::GetFrameHeight();
	const float spacing = ImGui::GetStyle().ItemSpacing.x;
	float comboWidth = currentAvailableWidthInGroup - buttonWidth - spacing;
	if (comboWidth < 100.0f) {
		comboWidth = 100.0f;
	}
	ImGui::SetNextItemWidth(comboWidth);

	auto& vehiclesModels = ModelsManager::getVehicleModels();
	if (ImGui::BeginCombo("##regularModelCombo", std::to_string(model).c_str())) {
		for (auto& item : vehiclesModels) {
			if (ImGui::Selectable(std::to_string(item).c_str(), item == model)) {
				model = item;
				dirty |= Model;
			}
		}
		ImGui::EndCombo();
	}
	ImGui::SameLine(0.0f, spacing);
	if (ImGui::Button(ICON_FA_CAR, ImVec2(buttonWidth, 0))) {
		PopupVehicleSelector::showPopup();
	}
	static std::function<void(int)> vehicleSelectorCallback = [this](int model) {
		this->model = model;
		dirty |= Model;
	};
	PopupVehicleSelector::renderPopup(vehicleSelectorCallback);
	ImGui::EndGroup();

	ImGui::Separator();

	ImGui::Text(tr("primary_color").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	const auto& selectedPrimaryColor = vehicleColorsData[primaryColorId];
	if (ImGui::BeginCombo("##primaryColorCombo", "")) {
		for (size_t i = 0; i < vehicleColorsData.size(); ++i) {
			const auto& colorData = vehicleColorsData[i];

			ImGui::PushID(static_cast<int>(i));

			// Color preview button (small square)
			ImGui::ColorButton("##colorPreview",
			                   ImVec4(colorData.r / 255.0f, colorData.g / 255.0f, colorData.b / 255.0f, 1.0f),
			                   ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoBorder,
			                   ImVec2(ImGui::GetFontSize(), ImGui::GetFontSize()));

			ImGui::SameLine();

			// Color name selectable
			if (ImGui::Selectable(colorData.name, i == primaryColorId, 0, ImVec2(0, 20))) {
				primaryColorId = static_cast<int>(i);
				dirty |= Model;
			}

			ImGui::PopID();
		}
		ImGui::EndCombo();
	}
	ImGui::SameLine();
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() - ImGui::GetItemRectSize().x - ImGui::GetStyle().ItemInnerSpacing.x);
	ImGui::ColorButton(
		"##selectedPrimaryColor",
		ImVec4(selectedPrimaryColor.r / 255.0f, selectedPrimaryColor.g / 255.0f, selectedPrimaryColor.b / 255.0f, 1.0f),
		ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoBorder, ImVec2(20, 20) * SCL_PX);
	ImGui::SameLine();
	ImGui::Text("%s", selectedPrimaryColor.name);

	ImGui::Text(tr("secondary_color").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	const auto& selectedSecondaryColor = vehicleColorsData[secondaryColorId];
	if (ImGui::BeginCombo("##secondaryColorCombo", "")) {
		for (size_t i = 0; i < vehicleColorsData.size(); ++i) {
			const auto& colorData = vehicleColorsData[i];

			ImGui::PushID(static_cast<int>(i));

			// Color preview button (small square)
			ImGui::ColorButton("##colorPreview",
			                   ImVec4(colorData.r / 255.0f, colorData.g / 255.0f, colorData.b / 255.0f, 1.0f),
			                   ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoBorder,
			                   ImVec2(ImGui::GetFontSize(), ImGui::GetFontSize()));

			ImGui::SameLine();

			// Color name selectable
			if (ImGui::Selectable(colorData.name, i == secondaryColorId, 0, ImVec2(0, 20))) {
				secondaryColorId = static_cast<int>(i);
				dirty |= Model;
			}

			ImGui::PopID();
		}
		ImGui::EndCombo();
	}
	ImGui::SameLine();
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() - ImGui::GetItemRectSize().x - ImGui::GetStyle().ItemInnerSpacing.x);
	ImGui::ColorButton("##selectedSecondaryColor",
	                   ImVec4(selectedSecondaryColor.r / 255.0f, selectedSecondaryColor.g / 255.0f,
	                          selectedSecondaryColor.b / 255.0f, 1.0f),
	                   ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoBorder, ImVec2(20, 20) * SCL_PX);
	ImGui::SameLine();
	ImGui::Text("%s", selectedSecondaryColor.name);

	ImGui::Text(tr("tertiary_color").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	const auto& selectedTertiaryColor = vehicleColorsData[tertiaryColorId];
	if (ImGui::BeginCombo("##tertiaryColorCombo", "")) {
		for (size_t i = 0; i < vehicleColorsData.size(); ++i) {
			const auto& colorData = vehicleColorsData[i];

			ImGui::PushID(static_cast<int>(i));

			// Color preview button (small square)
			ImGui::ColorButton("##colorPreview",
			                   ImVec4(colorData.r / 255.0f, colorData.g / 255.0f, colorData.b / 255.0f, 1.0f),
			                   ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoBorder,
			                   ImVec2(ImGui::GetFontSize(), ImGui::GetFontSize()));

			ImGui::SameLine();

			// Color name selectable
			if (ImGui::Selectable(colorData.name, i == tertiaryColorId, 0, ImVec2(0, 20))) {
				tertiaryColorId = static_cast<int>(i);
				dirty |= Model;
			}

			ImGui::PopID();
		}
		ImGui::EndCombo();
	}
	ImGui::SameLine();
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() - ImGui::GetItemRectSize().x - ImGui::GetStyle().ItemInnerSpacing.x);
	ImGui::ColorButton("##selectedTertiaryColor",
	                   ImVec4(selectedTertiaryColor.r / 255.0f, selectedTertiaryColor.g / 255.0f,
	                          selectedTertiaryColor.b / 255.0f, 1.0f),
	                   ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoBorder, ImVec2(20, 20) * SCL_PX);
	ImGui::SameLine();
	ImGui::Text("%s", selectedTertiaryColor.name);

	ImGui::Text(tr("quaternary_color").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	const auto& selectedQuaternaryColor = vehicleColorsData[quaternaryColorId];
	if (ImGui::BeginCombo("##quaternaryColorCombo", "")) {
		for (size_t i = 0; i < vehicleColorsData.size(); ++i) {
			const auto& colorData = vehicleColorsData[i];

			ImGui::PushID(static_cast<int>(i));

			// Color preview button (small square)
			ImGui::ColorButton("##colorPreview",
			                   ImVec4(colorData.r / 255.0f, colorData.g / 255.0f, colorData.b / 255.0f, 1.0f),
			                   ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoBorder,
			                   ImVec2(ImGui::GetFontSize(), ImGui::GetFontSize()));

			ImGui::SameLine();

			// Color name selectable
			if (ImGui::Selectable(colorData.name, i == quaternaryColorId, 0, ImVec2(0, 20))) {
				quaternaryColorId = static_cast<int>(i);
				dirty |= Model;
			}

			ImGui::PopID();
		}
		ImGui::EndCombo();
	}
	ImGui::SameLine();
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() - ImGui::GetItemRectSize().x - ImGui::GetStyle().ItemInnerSpacing.x);
	ImGui::ColorButton("##selectedQuaternaryColor",
	                   ImVec4(selectedQuaternaryColor.r / 255.0f, selectedQuaternaryColor.g / 255.0f,
	                          selectedQuaternaryColor.b / 255.0f, 1.0f),
	                   ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoBorder, ImVec2(20, 20) * SCL_PX);
	ImGui::SameLine();
	ImGui::Text("%s", selectedQuaternaryColor.name);

	ImGui::Separator();

	// Vehicle Properties Section
	ImGui::Text(tr("health").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::DragFloat("##health", &health, 1.0f, 0.0f, FLT_MAX, "%.0f", ImGuiSliderFlags_ClampOnInput);

	ImGui::Text(tr("locked").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::Checkbox("##locked", &locked);

	ImGui::Text(tr("must_survive").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::Checkbox("##mustSurvive", &mustSurvive);

	ImGui::Separator();

	// Vehicle Damage Resistance Section
	ImGui::Text(tr("bulletproof").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::Checkbox("##bulletproof", &bulletproof);

	ImGui::Text(tr("fireproof").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::Checkbox("##fireproof", &fireproof);

	ImGui::Text(tr("explosionproof").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::Checkbox("##explosionproof", &explosionproof);

	ImGui::Text(tr("collisionproof").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::Checkbox("##collisionproof", &collisionproof);

	ImGui::Text(tr("meleeproof").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::Checkbox("##meleeproof", &meleeproof);

	ImGui::Text(tr("tires_vulnerability").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::Checkbox("##tiresVulnerability", &tiresVulnerability);
}

void components::Vehicle::onStart() {
	Component::onStart();
	this->entity->setGetTransformCallbacks(
		[this]() -> float* {
			if (this->handle) {
				return (float*)&this->handle->GetMatrix()->pos;
			}
			return nullptr;
		},
		[this]() -> float* {
			// if (this->vehicle) {
		    // 	return (float*)&this->vehicle->GetMatrix()->rot;
		    // }
			return nullptr;
		},
		[this]() -> float* {
			// if (this->vehicle) {
		    // 	return (float*)&this->vehicle->GetMatrix()->scale;
		    // }
			return nullptr;
		});
	this->entity->setSetTransformCallbacks(
		[this](const float* position) {
			if (this->handle) {
				this->handle->SetPosn(position[0], position[1], position[2]);
			}
		},
		[this](const float* rotation) {}, [this](const float* scale) {});
	if (!IS_PLAYING) {
		spawn();
	} else {
		auto objectiveSpecific = ObjectiveSpecific::cast(this->entity->getComponent(ObjectiveSpecific::TYPE));
		if (objectiveSpecific) {
			this->onSpawnedConnection = std::make_optional(objectiveSpecific->onSpawned.connect([this]() { spawn(); }));
			this->onDespawnedConnection =
				std::make_optional(objectiveSpecific->onDespawned.connect([this]() { despawn(); }));
		}
	}
}

void components::Vehicle::onUpdate(float deltaTime) {
	Component::onUpdate(deltaTime);
	if (this->dirty & Direction) {
		updateDirection();
	}
	if (this->dirty & Position) {
		updatePosition();
	}
	if (this->dirty & Model) {
		spawn();
	}
	dirty = None;
}

void components::Vehicle::onReset() {
	Component::onReset();
	despawn();
	this->onSpawnedConnection.reset();
	this->onDespawnedConnection.reset();
}

void components::Vehicle::updateDirection() {
	if (this->handle) {
		auto heading = this->initialDirection * 180.0f / static_cast<float>(M_PI);
		plugin::Command<plugin::Commands::SET_CAR_HEADING>(this->getVehicleRef(), heading);
	}
}

void components::Vehicle::updatePosition() {
	if (this->handle) {
		auto& position = this->entity->position;
		this->handle->SetPosn(position[0], position[1], position[2]);
		this->handle->m_nPhysicalFlags.bSubmergedInWater = 0;
		this->handle->m_nPhysicalFlags.bOnSolidSurface = 0;
	}
}

void components::Vehicle::spawn() {
	int model = this->model;
	if (ModelsManager::validateVehicleModel(model)) {
		CStreaming::RequestModel(model, MISSION_REQUIRED);
	} else {
		return;
	}
	CStreaming::LoadAllRequestedModels(false);

	int newVehicle;
	auto& position = this->entity->position;
	plugin::Command<plugin::Commands::CREATE_CAR>(model, position[0], position[1], position[2], &newVehicle);
	CStreaming::SetMissionDoesntRequireModel(model);

	CVehicle* vehicle = CPools::GetVehicle(newVehicle);
	this->handle = std::shared_ptr<CVehicle>(vehicle, [](CVehicle* vehicle) {
		if (!ScenesManager::instance().isRestartGame()) {
			auto ref = CPools::GetVehicleRef(vehicle);
			plugin::Command<plugin::Commands::DELETE_CAR>(ref);
		}
	});
	updatePosition();
	updateDirection();
	vehicle->m_fHealth = this->health;
	vehicle->m_nPrimaryColor = this->primaryColorId;
	vehicle->m_nSecondaryColor = this->secondaryColorId;
	vehicle->m_nTertiaryColor = this->tertiaryColorId;
	vehicle->m_nQuaternaryColor = this->quaternaryColorId;
	vehicle->m_eDoorLock = this->locked ? DOORLOCK_LOCKED : DOORLOCK_UNLOCKED;

	if (this->mustSurvive) {
		ProjectPlayer::instance().projectTasklist->add_task(
			[](const Vehicle* _this) -> ktwait {
				while (_this->handle && IS_PLAYING) {
					if (plugin::Command<plugin::Commands::IS_CAR_DEAD>(_this->getVehicleRef())) {
						ProjectPlayer::instance().failCurrentProject();
						break;
					}
					co_await 1;
				}
			},
			this);
	}
	plugin::Command<plugin::Commands::SET_VEHICLE_AREA_VISIBLE>(newVehicle, this->entity->areaId);

	vehicle->m_nPhysicalFlags.bBulletProof = static_cast<unsigned>(this->bulletproof);
	vehicle->m_nPhysicalFlags.bCollisionProof = static_cast<unsigned>(this->collisionproof);
	vehicle->m_nPhysicalFlags.bExplosionProof = static_cast<unsigned>(this->explosionproof);
	vehicle->m_nPhysicalFlags.bFireProof = static_cast<unsigned>(this->fireproof);
	vehicle->m_nPhysicalFlags.bMeleeProof = static_cast<unsigned>(this->meleeproof);
	plugin::Command<plugin::Commands::SET_CAN_BURST_CAR_TYRES>(newVehicle, !this->tiresVulnerability);

	if (!IS_PLAYING) {
		vehicle->m_bUsesCollision = 0;
		vehicle->m_nPhysicalFlags.bBulletProof = 1;
		vehicle->m_nPhysicalFlags.bCollisionProof = 1;
		vehicle->m_nPhysicalFlags.bExplosionProof = 1;
		vehicle->m_nPhysicalFlags.bFireProof = 1;
		vehicle->m_nPhysicalFlags.bMeleeProof = 1;
		plugin::Command<plugin::Commands::FREEZE_CAR_POSITION_AND_DONT_LOAD_COLLISION>(newVehicle, 1);
	}

	onSpawned();
}

void components::Vehicle::despawn() {
	if (handle) {
		handle = nullptr;
	}
	onDespawned();
}