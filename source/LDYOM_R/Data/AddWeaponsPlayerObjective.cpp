﻿#define NOMINMAX
#include "AddWeaponsPlayerObjective.h"

#include <CStreaming.h>
#include <CWeaponInfo.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "common.h"
#include "fa.h"
#include "imgui.h"
#include "ModelsService.h"
#include "strUtils.h"
#include "../Windows/PopupWeaponSelector.h"

AddWeaponsPlayerObjective::AddWeaponsPlayerObjective(void *_new): BaseObjective(_new) {
	const auto suffix = fmt::format(" : {}", Localization::getInstance().get("objective.add_weapons_player"));
	this->name += suffix;
}

std::vector<Weapon>& AddWeaponsPlayerObjective::getWeapons() { return weapons_; }

int& AddWeaponsPlayerObjective::getDefaultWeapon() { return defaultWeapon_; }

void AddWeaponsPlayerObjective::draw(Localization &local, std::vector<std::string> &listOverlay) {
	if (ImGui::BeginTable("##weaponsTable", 4, ImGuiTableFlags_ScrollY, ImVec2(300.0f, 200.0f))) {
		ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 60.0f);
		ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 120.0f);
		ImGui::TableSetupColumn(local.get("general.initial").c_str());
		ImGui::TableSetupColumn("");
		ImGui::TableSetupScrollFreeze(4, 1);
		ImGui::TableHeadersRow();

		for (int i = 0; i < static_cast<int>(this->weapons_.size()); ++i) {
			auto &weapon = this->weapons_.at(i);

			ImGui::PushID(i);

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			PopupWeaponSelector::getInstance().weaponButton(&weapon.weapon);

			PopupWeaponSelector::getInstance().renderPopup([&weapon](const int weaponId) {
				weapon.weapon = weaponId;
			});


			ImGui::TableNextColumn();
			ImGui::BeginGroup();

			ImGui::InputInt(local.get("general.model").c_str(), &weapon.weapon, 0, 0);
			ImGui::InputInt(local.get("weapons.ammo").c_str(), &weapon.ammo, 0, 0);
			ImGui::EndGroup();

			ImGui::TableNextColumn();
			ImGui::RadioButton(fmt::format("##{}", i).c_str(), &this->defaultWeapon_, i);

			ImGui::TableNextColumn();
			if (ImGui::Button(ICON_FA_TRASH, ImVec2(25.0f, 0.0f))) {
				this->weapons_.erase(this->weapons_.begin() + i);
				i = static_cast<int>(this->weapons_.size());
				this->defaultWeapon_ = std::max(std::min(static_cast<int>(this->weapons_.size()) - 1,
				                                         this->defaultWeapon_), 0);
			}

			ImGui::PopID();
		}
		ImGui::EndTable();
	}
	if (ImGui::Button(local.get("general.add").c_str(), ImVec2(ImGui::GetContentRegionAvail().x - 1.0f, 0.0f))) {
		this->weapons_.emplace_back(1, 0);
	}
}

ktwait AddWeaponsPlayerObjective::execute(Scene *scene, Result &result, ktcoro_tasklist &tasklist) {
	for (const auto &[weaponRaw, ammo] : this->weapons_) {
		int weapon = ModelsService::validWeaponModel(weaponRaw);
		const int weaponModel = CWeaponInfo::GetWeaponInfo(static_cast<eWeaponType>(weapon), 1)->m_nModelId1;

		CStreaming::RequestModel(weaponModel, GAME_REQUIRED);
		CStreaming::LoadAllRequestedModels(false);

		FindPlayerPed()->GiveWeapon(static_cast<eWeaponType>(weapon), ammo, false);

		CStreaming::SetMissionDoesntRequireModel(weaponModel);
	}
	if (!this->weapons_.empty()) {
		int currentWeapon = this->weapons_.at(this->defaultWeapon_).weapon;
		FindPlayerPed()->SetCurrentWeapon(static_cast<eWeaponType>(currentWeapon));
	}
	co_return;
}
