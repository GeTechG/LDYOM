﻿#include "SettingsWindow.h"
#include "imgui.h"
#include "fa.h"
#include <fmt/core.h>

#include "HotKeyService.h"
#include "Settings.h"
#include "utils.h"
#include "Localization/Localization.h"
#include "imHotKey.h"
#include "WindowsRenderService.h"
#include "imgui_stdlib.h"
#include "ModelsService.h"

using namespace std;

namespace Windows
{
	void SettingsWindow::Init()
	{
		settingsUpdate();
		Settings::getInstance().onUpdate().connect([this] { settingsUpdate(); });
	}

	void SettingsWindow::settingsUpdate()
	{
		this->settings_.currentLanguage = ranges::find(Settings::getInstance().listLocalizations(), Settings::getInstance().get<std::string>("main.language")) - Settings::getInstance().listLocalizations().begin();
		this->settings_.currentTheme = ranges::find(Settings::getInstance().listThemes(), Settings::getInstance().get<std::string>("main.theme")) - Settings::getInstance().listThemes().begin();
		this->settings_.showEntitiesName = Settings::getInstance().get<bool>("main.showEntitiesName").value_or(false);
		this->settings_.distanceShowNames = Settings::getInstance().get<float>("main.distanceShowNames").value_or(100.0f);

		if (const auto userPedsModels = Settings::getInstance().get<std::vector<int>>("data.pedModels"); userPedsModels.has_value())
			this->settings_.userPedsModels = userPedsModels.value();
		if (const auto userSpecialPedsModels = Settings::getInstance().get<std::vector<std::string>>("data.specialPedModels"); userSpecialPedsModels.has_value())
			this->settings_.userSpecialPedsModels = userSpecialPedsModels.value();
		if (const auto userVehiclesModels = Settings::getInstance().get<std::vector<int>>("data.vehicleModels"); userVehiclesModels.has_value())
			this->settings_.userVehiclesModels = userVehiclesModels.value();
		if (const auto userWeaponsModels = Settings::getInstance().get<std::vector<int>>("data.weaponModels"); userWeaponsModels.has_value())
			this->settings_.userWeaponsModels = userWeaponsModels.value();

		if (const auto userPedAnimations = Settings::getInstance().get<std::map<std::string, std::vector<std::string>>>("data.pedAnimations"); userPedAnimations.has_value()) {
			for (const auto &pair : userPedAnimations.value()) {
				for (const auto &value : pair.second) {
					this->settings_.userPedAnimations.emplace_back(pair.first, value);
				} 
			} 
		}
			
	}

	void renderModel(Localization& local, const char* name, int vecSize, const std::function<void()>& createElement, const std::function<void(int)>& eraseElement, const std::function<void(int)>& renderElement) {
		if (ImGui::TreeNode(name)) {
			if (ImGui::BeginChild(reinterpret_cast<unsigned>(name), ImVec2(ImGui::GetContentRegionAvail().x, 100.0f), true)) {
				for (int i = 0; i < vecSize; ++i) {
					ImGui::PushID(i);
					ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - 30.0f);
					renderElement(i);
					ImGui::SameLine();
					if (ImGui::Button(ICON_FA_TRASH_ALT, ImVec2(25.0f, 0.0f))) {
						eraseElement(i);
						i = vecSize;
					}
					ImGui::PopID();
				}
			}
			ImGui::EndChildFrame();
			if (ImGui::Button(local.get("general.add").c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 0.0f))) {
				createElement();
			}
			ImGui::TreePop();
		}
	}

	void renderModel(Localization& local, const char* name, std::vector<int>& models) {
		renderModel(local, name, static_cast<int>(models.size()), [&] {
			models.emplace_back(0);
		}, [&](int i) {
			models.erase(models.begin() + i);
		}, [&](int i) {
			ImGui::InputInt("##modelID", &models.at(i), 0, 0);
		});
	}

	void renderModel(Localization& local, const char* name, std::vector<std::string>& models) {
		renderModel(local, name, static_cast<int>(models.size()), [&] {
			models.emplace_back("");
			}, [&](int i) {
				models.erase(models.begin() + i);
			}, [&](int i) {
				ImGui::InputText("##modelID", &models.at(i));
			});
	}

	void renderModel(Localization& local, const char* name, std::vector<std::pair<std::string, std::string>>& models) {
		renderModel(local, name, static_cast<int>(models.size()), [&] {
			models.emplace_back(std::make_pair("", ""));
			}, [&](int i) {
				models.erase(models.begin() + i);
			}, [&](int i) {
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x / 2.0f - 15.0f);
				ImGui::InputTextWithHint("##ifpFile", "IFP", &models.at(i).first);
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x / 2.0f - 15.0f);
				ImGui::SameLine();
				ImGui::InputTextWithHint("##animName", "anim_name", &models.at(i).second);
			});
	}

	void SettingsWindow::draw()
	{
		static auto& local = Localization::getInstance();

		const ImVec2 displaySize = ImGui::GetIO().DisplaySize;
		ImGui::SetNextWindowPos(ImVec2(displaySize.x / 2.f, displaySize.y / 2.f), ImGuiCond_Appearing, ImVec2(.5f, .5f));
		if (ImGui::Begin(fmt::format("{} {}", ICON_FA_COGS, local.get("settings.title")).c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::SetNextItemWidth(150.0f);
			if (utils::Combo(local.get("settings.languages").c_str(), &this->settings_.currentLanguage, Settings::getInstance().listLocalizations()))
			{
				Settings::getInstance().set("main.language", Settings::getInstance().listLocalizations().at(this->settings_.currentLanguage));
				Localization::getInstance().Update();
				Settings::getInstance().Save();
			}
			ImGui::SetNextItemWidth(150.0f);
			if (utils::Combo(local.get("settings.themes").c_str(), &this->settings_.currentTheme, Settings::getInstance().listThemes()))
			{
				Settings::getInstance().set("main.theme", Settings::getInstance().listThemes().at(this->settings_.currentTheme));
				WindowsRenderService::getInstance().style();
				Settings::getInstance().Save();
			}

			if (utils::ToggleButton(local.get("settings.show_entities_names").c_str(), &this->settings_.showEntitiesName)) {
				Settings::getInstance().set("main.showEntitiesName", this->settings_.showEntitiesName);
				Settings::getInstance().Save();
			}

			ImGui::SetNextItemWidth(150.0f);
			if (ImGui::InputFloat(local.get("settings.distance_show_names").c_str(), &this->settings_.distanceShowNames, 0.0f, 0.0f)) {
				Settings::getInstance().set("main.distanceShowNames", this->settings_.distanceShowNames);
				Settings::getInstance().Save();
			}

			ImGui::Separator();

			renderModel(local, local.get("settings.custom_ped_models").c_str(), this->settings_.userPedsModels);
			renderModel(local, local.get("settings.custom_special_ped_models").c_str(), this->settings_.userSpecialPedsModels);
			renderModel(local, local.get("settings.custom_vehicle_models").c_str(), this->settings_.userVehiclesModels);
			renderModel(local, local.get("settings.custom_weapon_models").c_str(), this->settings_.userWeaponsModels);
			renderModel(local, local.get("settings.custom_ped_animations").c_str(), this->settings_.userPedAnimations);

			if (ImGui::Button(local.get("general.save").c_str())) {
				Settings::getInstance().set("data.pedModels", this->settings_.userPedsModels);
				Settings::getInstance().set("data.specialPedModels", this->settings_.userSpecialPedsModels);
				Settings::getInstance().set("data.vehicleModels", this->settings_.userVehiclesModels);
				Settings::getInstance().set("data.weaponModels", this->settings_.userWeaponsModels);

				std::map<std::string, std::vector<std::string>> anims;
				for (const auto &pair : this->settings_.userPedAnimations) {
					if (anims.contains(pair.first))
						anims[pair.first].emplace_back(pair.second);
					else
						anims.emplace(pair.first, std::vector{pair.second});
				}
				Settings::getInstance().set("data.pedAnimations", anims);

				Settings::getInstance().Save();
				ModelsService::getInstance().Update();
			}
			ImGui::Separator();

			ImGui::SetNextItemWidth(150.0f);
			if (ImGui::Button(fmt::format("{} {}", ICON_FA_FIRE, local.get("settings.edit_hotkeys").c_str()).c_str()))
			{
				ImGui::OpenPopup(local.get("hotkey_editor.title").c_str());
			}
			ImHotKey::Edit(HotKeyService::getInstance().getHotkeys().data(),
			               HotKeyService::getInstance().getHotkeys().size(), local.get("hotkey_editor.title").c_str(),
			               [](ImHotKey::HotKey& hotKey) { HotKeyService::getInstance().onEditHotKey(hotKey); });

		}
		ImGui::End();
	}
}