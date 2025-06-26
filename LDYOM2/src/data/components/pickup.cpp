#define IMGUI_DEFINE_MATH_OPERATORS
#include "pickup.h"
#include "extensions/ScriptCommands.h"
#include "objective_specific.h"
#include <CPickups.h>
#include <CStreaming.h>
#include <corecrt_math_defines.h>
#include <entity.h>
#include <lua_define_type.h>
#include <popups/vehicle_selector.h>
#include <popups/weapon_selector.h>
#include <project_player.h>
#include <scenes_manager.h>
#include <string_utils.h>
#include <textures_manager.h>
#include <utils/imgui_configurate.h>

void components::Pickup::sol_lua_register(sol::state_view lua_state) {
	auto ut = lua_state.new_usertype<Pickup>("PickupComponent");
	SOL_LUA_FOR_EACH(SOL_LUA_BIND_MEMBER_ACTION, ut, components::Pickup, cast, pickupType, spawnType, weaponType, ammo,
	                 model);
}

components::Pickup::Pickup()
	: Component(TYPE) {}

components::Pickup::~Pickup() {
	if (this->handle != -1) {
		this->despawn();
	}
}

inline nlohmann::json components::Pickup::to_json() const {
	auto j = this->Component::to_json();
	j["pickupType"] = pickupType;
	j["spawnType"] = spawnType;
	j["weaponType"] = weaponType;
	j["ammo"] = ammo;
	j["model"] = model;
	return j;
}

void components::Pickup::from_json(const nlohmann::json& j) {
	this->Component::from_json(j);
	j.at("pickupType").get_to(pickupType);
	j.at("spawnType").get_to(spawnType);
	j.at("weaponType").get_to(weaponType);
	j.at("ammo").get_to(ammo);
	j.at("model").get_to(model);
}

void components::Pickup::editorRender() {
	const auto availableWidth = ImGui::GetContentRegionAvail().x;

	ImGui::Text(tr("type").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	if (ImGui::BeginCombo("##type", tr(fmt::format("types.{}", pickupType)).c_str())) {
		for (int i = 0; i < 5; i++) {
			if (ImGui::Selectable(tr(fmt::format("types.{}", i)).c_str(), pickupType == i)) {
				this->pickupType = i;
				this->dirty = true;
			}
			if (pickupType == i) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	ImGui::Text(tr("spawn_type").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	if (ImGui::BeginCombo("##spawn_type", tr(fmt::format("spawn_types.{}", spawnType)).c_str())) {
		for (int i = 0; i < 3; i++) {
			if (ImGui::Selectable(tr(fmt::format("spawn_types.{}", i)).c_str(), spawnType == i)) {
				this->spawnType = i;
				this->dirty = true;
			}
			if (pickupType == i) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	bool openEditPopup = false;

	if (pickupType == 0) { // weapon
		ImGui::Text(_("weapon").c_str());
		ImGui::SameLine(availableWidth * 0.35f);
		{
			auto icon = ModelsManager::validateWeaponId(weaponType)
			                ? PopupWeaponSelector::getWeaponIcon(weaponType)
			                : TexturesManager::instance().getTexture("50px-Weapon-unk-hd").value();
			if (icon->getTexture() == nullptr) {
				icon = TexturesManager::instance().getTexture("50px-Weapon-unk-hd").value();
			}
			auto size = ImVec2(static_cast<float>(icon->getWidth()), static_cast<float>(icon->getHeight())) * SCL_PX;
			if (ImGui::ImageButton(std::to_string(weaponType).c_str(), (ImTextureID)icon->getTexture(), size,
			                       ImVec2(0, 0), ImVec2(1, 1), ImVec4(0, 0, 0, 0))) {
				openEditPopup = true;
			}
		}

		ImGui::Text(_("ammo").c_str());
		ImGui::SameLine(availableWidth * 0.35f);
		ImGui::SetNextItemWidth(-1.f);
		ImGui::InputInt("##ammo", &ammo, 1, 10);

		// Ensure ammo is not negative
		if (ammo < 0)
			ammo = 0;
	} else if (pickupType == 4) { // object
		ImGui::Text(_("model").c_str());
		ImGui::SameLine(availableWidth * 0.35f);
		ImGui::SetNextItemWidth(-1.f);
		if (ImGui::InputInt("##model", &model)) {
			if (model < 0) {
				model = 0;
			}
			this->dirty = true;
		}
	}

	static bool selectWeaponPopup = false;
	if (openEditPopup) {
		PopupWeaponSelector::showPopup("select_weapon");
		selectWeaponPopup = true;
	}

	PopupWeaponSelector::renderPopup("select_weapon", &selectWeaponPopup, [this](int weaponId) {
		weaponType = weaponId;
		dirty = true;
	});
}

void components::Pickup::onStart() {
	Component::onStart();
	this->entity->setGetTransformCallbacks(
		[this]() -> std::array<float, 3> {
			if (this->handle != -1) {
				auto actualIndex = CPickups::GetActualPickupIndex(handle);
				if (actualIndex != -1) {
					auto position = CPickups::aPickUps[actualIndex].GetPosn();
					return {position.x, position.y, position.z};
				}
			}
			return {0.0f, 0.0f, 0.0f};
		},
		[this]() -> std::array<float, 3> {
			// if (this->vehicle) {
		    // 	return (std::array<float, 3>)&this->vehicle->GetMatrix()->rot;
		    // }
			return {0.0f, 0.0f, 0.0f};
		},
		[this]() -> std::array<float, 3> {
			// if (this->vehicle) {
		    // 	return (std::array<float, 3>)&this->vehicle->GetMatrix()->scale;
		    // }
			return {0.0f, 0.0f, 0.0f};
		});
	this->entity->setSetTransformCallbacks(
		[this](const std::array<float, 3> position) {
			if (this->handle != -1) {
				auto actualIndex = CPickups::GetActualPickupIndex(handle);
				CPickups::aPickUps[actualIndex].SetPosn(position[0], position[1], position[2]);
			}
		},
		[this](const std::array<float, 3> rotation) {}, [this](const std::array<float, 3> scale) {});
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

void components::Pickup::onUpdate(float deltaTime) {
	Component::onUpdate(deltaTime);
	if (this->dirty) {
		spawn();
	}
}

void components::Pickup::onReset() {
	Component::onReset();
	despawn();
	this->onSpawnedConnection.reset();
	this->onDespawnedConnection.reset();
}

void components::Pickup::spawn() {
	despawn();

	int pickupType = 3;
	if (IS_PLAYING) {
		switch (this->spawnType) {
			case 1: // every 30 seconds
				pickupType = 2;
				break;
			case 2: // every 6 minutes
				pickupType = 15;
			default: break;
		}
	} else {
		pickupType = 9;
	}

	if (this->pickupType == 0) { // weapon
		if (ModelsManager::validateWeaponId(this->weaponType)) {
			const int weaponModel =
				CWeaponInfo::GetWeaponInfo(static_cast<eWeaponType>(this->weaponType), 1)->m_nModelId;
			if (weaponModel != -1) {
				CStreaming::RequestModel(weaponModel, MISSION_REQUIRED);
				CStreaming::LoadAllRequestedModels(false);
			}
			CVector pos(this->entity->position[0], this->entity->position[1], this->entity->position[2]);
			this->handle = CPickups::GenerateNewOne_WeaponType(pos, static_cast<eWeaponType>(this->weaponType),
			                                                   pickupType, ammo, false, nullptr);
			CStreaming::SetMissionDoesntRequireModel(weaponModel);
		}
	} else {
		int modelId = this->model;
		switch (pickupType) {
			case 1: // health
				modelId = 1240;
				break;
			case 2: // armour
				modelId = 1242;
				break;
			case 3: // police bribe
				modelId = 1247;
				break;
			default: break;
		}

		CStreaming::RequestModel(modelId, MISSION_REQUIRED);
		CStreaming::LoadAllRequestedModels(false);

		CVector pos(this->entity->position[0], this->entity->position[1], this->entity->position[2]);
		this->handle = CPickups::GenerateNewOne(pos, modelId, pickupType, this->ammo, 0, false, nullptr);
		CStreaming::SetMissionDoesntRequireModel(modelId);
	}

	onSpawned();
}

void components::Pickup::despawn() {
	if (handle != -1) {
		CPickups::RemovePickUp(handle);
		handle = -1;
	}
	onDespawned();
}