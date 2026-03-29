#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS
#include "objective.h"
#include <algorithm>
#include <CPlayerPed.h>
#include <extensions/ScriptCommands.h>
#include <fa_icons.h>
#include <imgui.h>
#include <ktcoro_wait.hpp>
#include <localization.h>
#include <models_manager.h>
#include <objectives_manager.h>
#include <task_manager.h>
#include <utils/imgui_configurate.h>

namespace objectives::player_animation {
constexpr const char* TYPE = "core.player_animation";

struct Data {
	std::string packName = "PED";
	std::string animName = "IDLE_ARMED";
	bool isLooped = true;

	static void sol_lua_register(sol::state_view lua_state) {
		auto ut = lua_state.new_usertype<Data>("ObjectivePlayerAnimationData");
		SOL_LUA_FOR_EACH(SOL_LUA_BIND_MEMBER_ACTION, ut, Data, packName, animName, isLooped);
	}
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Data, packName, animName, isLooped);
};

inline std::string tr(const std::string& key) { return _(fmt::format("objectives.{}.{}", std::string(TYPE), key)); }

void playAnimation(const Data& data) {
	auto callback = [](const Data* data) -> ktwait {
		auto player = FindPlayerPed();
		if (!player) {
			co_return;
		}

		if (ModelsManager::validatePedAnimation(data->packName, data->animName)) {
			const auto isCustom = data->packName != "PED";
			if (isCustom) {
				plugin::Command<plugin::Commands::REQUEST_ANIMATION>(data->packName.c_str());
				while (!plugin::Command<plugin::Commands::HAS_ANIMATION_LOADED>(data->packName.c_str())) {
					co_await 1;
				}
			}

			plugin::Command<plugin::Commands::TASK_PLAY_ANIM>(player, data->animName.c_str(), data->packName.c_str(),
			                                                  4.0f, data->isLooped, !data->isLooped, !data->isLooped,
			                                                  false, -1);

			if (isCustom) {
				plugin::Command<plugin::Commands::REMOVE_ANIMATION>(data->packName.c_str());
			}
		}
		co_await 0;
	};
	TaskManager::instance().addTask("PlayerAnimation::playAnimation", callback, &data);
}

void stopAnimation() {
	auto player = FindPlayerPed();
	if (player) {
		plugin::Command<plugin::Commands::CLEAR_CHAR_TASKS>(player);
	}
}

bool isPlayingAnimation(const Data& data) {
	auto player = FindPlayerPed();
	if (player) {
		return plugin::Command<plugin::Commands::IS_CHAR_PLAYING_ANIM>(player, data.animName.c_str());
	}
	return false;
}

void renderEditor(Data& data) {
	ImGui::Dummy(ImVec2((SCL_PX).x * 300.f, 0.f)); // Add some space at the top
	const auto availableWidth = ImGui::GetContentRegionAvail().x;

	static char packFilter[64] = {};
	static char animFilter[64] = {};

	ImGui::Text(tr("pack_name").c_str());
	ImGui::SameLine(availableWidth * 0.6f);
	ImGui::SetNextItemWidth(-1.f);
	if (ImGui::BeginCombo("##packName", data.packName.c_str(), ImGuiComboFlags_HeightLargest)) {
		const bool justOpened = ImGui::IsWindowAppearing();
		if (justOpened) {
			packFilter[0] = '\0';
			ImGui::SetKeyboardFocusHere();
		}
		ImGui::SetNextItemWidth(-1.f);
		ImGui::InputText("##packFilter", packFilter, sizeof(packFilter));
		ImGui::Separator();
		const float listHeight = ImGui::GetTextLineHeightWithSpacing() * 10;
		if (ImGui::BeginChild("##packList", ImVec2(0, listHeight))) {
			const std::string_view filterView = packFilter;
			for (const auto& key : ModelsManager::getPedAnimations() | std::views::keys) {
				if (!filterView.empty()) {
					std::string keyLower = key;
					std::string filterLower = packFilter;
					std::transform(keyLower.begin(), keyLower.end(), keyLower.begin(), ::tolower);
					std::transform(filterLower.begin(), filterLower.end(), filterLower.begin(), ::tolower);
					if (keyLower.find(filterLower) == std::string::npos)
						continue;
				}
				const bool isSelected = key == data.packName;
				if (ImGui::Selectable(key.c_str(), isSelected)) {
					const bool wasPlaying = isPlayingAnimation(data);
					data.packName = key;
					data.animName = ModelsManager::getPedAnimations().at(key).front();
					if (data.isLooped && wasPlaying) {
						stopAnimation();
						playAnimation(data);
					}
				}
				if (isSelected && justOpened) {
					ImGui::SetScrollHereY(0.5f);
				}
			}
		}
		ImGui::EndChild();
		ImGui::EndCombo();
	}

	ImGui::Text(tr("anim_name").c_str());
	ImGui::SameLine(availableWidth * 0.6f);
	ImGui::SetNextItemWidth(-1.f);
	if (ImGui::BeginCombo("##animName", data.animName.c_str(), ImGuiComboFlags_HeightLargest)) {
		const bool justOpened = ImGui::IsWindowAppearing();
		if (justOpened) {
			animFilter[0] = '\0';
			ImGui::SetKeyboardFocusHere();
		}
		ImGui::SetNextItemWidth(-1.f);
		ImGui::InputText("##animFilter", animFilter, sizeof(animFilter));
		ImGui::Separator();
		const float listHeight = ImGui::GetTextLineHeightWithSpacing() * 10;
		if (ImGui::BeginChild("##animList", ImVec2(0, listHeight))) {
			const std::string_view filterView = animFilter;
			if (ModelsManager::getPedAnimations().contains(data.packName)) {
				for (const auto& anim : ModelsManager::getPedAnimations().at(data.packName)) {
					if (!filterView.empty()) {
						std::string animLower = anim;
						std::string filterLower = animFilter;
						std::transform(animLower.begin(), animLower.end(), animLower.begin(), ::tolower);
						std::transform(filterLower.begin(), filterLower.end(), filterLower.begin(), ::tolower);
						if (animLower.find(filterLower) == std::string::npos)
							continue;
					}
					const bool isSelected = anim == data.animName;
					if (ImGui::Selectable(anim.c_str(), isSelected)) {
						const bool wasPlaying = isPlayingAnimation(data);
						data.animName = anim;
						if (data.isLooped && wasPlaying) {
							stopAnimation();
							playAnimation(data);
						}
					}
					if (isSelected && justOpened) {
						ImGui::SetScrollHereY(0.5f);
					}
				}
			}
		}
		ImGui::EndChild();
		ImGui::EndCombo();
	}

	ImGui::Text(tr("is_looped").c_str());
	ImGui::SameLine(availableWidth * 0.6f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::Checkbox("##isLooped", &data.isLooped);

	if (isPlayingAnimation(data)) {
		if (ImGui::Button(fmt::format("{} {}", ICON_FA_STOP, tr("stop_animation")).c_str())) {
			stopAnimation();
		}
	} else {
		if (ImGui::Button(fmt::format("{} {}", ICON_FA_PLAY, tr("play_animation")).c_str())) {
			playAnimation(data);
		}
	}
}

inline ktwait execute(Data& data) {
	auto player = FindPlayerPed();
	if (!player) {
		co_return;
	}

	// Validate and play animation (same logic as ActorAnimation component)
	if (ModelsManager::validatePedAnimation(data.packName, data.animName)) {
		const auto isCustom = data.packName != "PED";
		if (isCustom) {
			plugin::Command<plugin::Commands::REQUEST_ANIMATION>(data.packName.c_str());
			while (!plugin::Command<plugin::Commands::HAS_ANIMATION_LOADED>(data.packName.c_str())) {
				co_await 1;
			}
		}

		plugin::Command<plugin::Commands::TASK_PLAY_ANIM>(player, data.animName.c_str(), data.packName.c_str(), 4.0f,
		                                                  data.isLooped, !data.isLooped, !data.isLooped, false, -1);

		if (isCustom) {
			plugin::Command<plugin::Commands::REMOVE_ANIMATION>(data.packName.c_str());
		}
	}

	co_return;
}

Objective make() {
	return Objective(TYPE, _("objectives." + std::string(TYPE) + ".name"), Data{},
	                 std::function<void(Data&)>(renderEditor), std::function<ktwait(Data&)>(execute));
}

ObjectiveBuilderData builder() { return ObjectiveBuilderData{.type = TYPE, .category = "player", .builder = make}; }
} // namespace objectives::player_animation
