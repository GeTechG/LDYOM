#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS
#include "objective.h"
#include <CMessages.h>
#include <CTheScripts.h>
#include <blip_color_utils.h>
#include <common.h>
#include <extensions/ScriptCommands.h>
#include <fstream>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <imgui_widgets/imgui_widgets.h>
#include <ktcoro_wait.hpp>
#include <localization.h>
#include <logger.h>
#include <notifications.h>
#include <objectives_manager.h>
#include <project_player.h>
#include <projects_manager.h>
#include <scenes_manager.h>
#include <string_utils.h>
#include <utils/imgui_configurate.h>
#include <utils_entities_selections.h>

namespace objectives::mission_start {
constexpr const char* TYPE = "core.mission_start";

struct Data {
	std::string titleText;
	float titleTime = 1.f;
	bool fadeIn = false;
	int failTextMode = 0; // 0 = Default (M_FAIL), 1 = Custom
	std::string failText;
	MissionFailAction failAction = mission_fail_actions::EndProject{};
	std::string gameTitleText; // cached gxt for the title banner; not persisted

	static void sol_lua_register(sol::state_view lua_state) {
		using namespace mission_fail_actions;

		// ── MissionFailEndProject ──────────────────────────────────────────────
		lua_state.new_usertype<EndProject>(
			"MissionFailEndProject",
			sol::factories([]() { return EndProject{}; }),
			"fadeOut", &EndProject::fadeOut
		);

		// ── MissionFailRestartScene ────────────────────────────────────────────
		lua_state.new_usertype<RestartScene>(
			"MissionFailRestartScene",
			sol::factories([]() { return RestartScene{}; }),
			"fadeOut", &RestartScene::fadeOut
		);

		// ── MissionFailGotoScene ───────────────────────────────────────────────
		lua_state.new_usertype<GotoScene>(
			"MissionFailGotoScene",
			sol::factories([]() { return GotoScene{}; }),
			"fadeOut", &GotoScene::fadeOut,
			"sceneId", &GotoScene::sceneId,
			"objectiveId", sol::property(
				[](GotoScene& g, sol::this_state s) -> sol::object {
					if (g.objectiveId.has_value())
						return sol::make_object(s, *g.objectiveId);
					return sol::make_object(s, sol::lua_nil);
				},
				[](GotoScene& g, sol::object v) {
					if (v.get_type() == sol::type::lua_nil)
						g.objectiveId = std::nullopt;
					else
						g.objectiveId = v.as<std::string>();
				}
			)
		);

		// ── ObjectiveMissionStartData ──────────────────────────────────────────
		auto ut = lua_state.new_usertype<Data>("ObjectiveMissionStartData");
		SOL_LUA_FOR_EACH(SOL_LUA_BIND_MEMBER_ACTION, ut, Data, titleText, titleTime, fadeIn, failTextMode, failText);

		// failAction — variant property; getter returns a copy of the active alternative as its usertype,
		// setter accepts any of the three MissionFail* usertypes and assigns the whole variant.
		ut["failAction"] = sol::property(
			[](Data& d, sol::this_state s) -> sol::object {
				sol::state_view lua(s);
				return std::visit([&lua](const auto& a) { return sol::make_object(lua, a); }, d.failAction);
			},
			[](Data& d, sol::object value) {
				using namespace mission_fail_actions;
				if (value.is<EndProject>())
					d.failAction = value.as<EndProject>();
				else if (value.is<RestartScene>())
					d.failAction = value.as<RestartScene>();
				else if (value.is<GotoScene>())
					d.failAction = value.as<GotoScene>();
				else
					throw sol::error("mission failAction expects MissionFailEndProject, MissionFailRestartScene, or MissionFailGotoScene");
			}
		);
	}
	NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Data, titleText, titleTime, fadeIn, failTextMode, failText, failAction);
};

inline std::string tr(const std::string& key) { return _(fmt::format("objectives.{}.{}", std::string(TYPE), key)); }

inline void renderEditor(Data& data) {
	ImGui::Dummy(ImVec2((SCL_PX).x * 300.f, 0.f));

	ImGui::Text(tr("title_text").c_str());
	ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::InputTextWithPopup("##title_text", &data.titleText);

	ImGui::Text(tr("title_time").c_str());
	ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::DragFloat("##title_time", &data.titleTime, 0.1f, 0.f, 10.f, "%.1f");

	ImGui::Text(tr("fade_in").c_str());
	ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.45f);
	ImGui::Checkbox("##fade_in", &data.fadeIn);

	ImGui::Text(tr("fail_text_mode").c_str());
	ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	constexpr int failModeCount = 2;
	if (ImGui::BeginCombo("##fail_text_mode", tr(fmt::format("fail_text_modes.{}", data.failTextMode)).c_str())) {
		for (int i = 0; i < failModeCount; i++) {
			const bool isSelected = (data.failTextMode == i);
			if (ImGui::Selectable(tr(fmt::format("fail_text_modes.{}", i)).c_str(), isSelected)) {
				data.failTextMode = i;
			}
			if (isSelected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	if (data.failTextMode == 1) {
		ImGui::Text(tr("fail_text").c_str());
		ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.45f);
		ImGui::SetNextItemWidth(-1.f);
		ImGui::InputTextWithPopup("##fail_text", &data.failText);
	}

	// Derive current action index from the active variant alternative
	int currentAction = static_cast<int>(data.failAction.index());
	// Localization keys for each alternative (ordered to match variant index: 0=EndProject, 1=RestartScene, 2=GotoScene)
	const char* failActionKeys[] = {
		"fail_action_end_project",
		"fail_action_restart_scene",
		"fail_action_goto_scene",
	};
	ImGui::Text(tr("fail_action").c_str());
	ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	constexpr int failActionCount = 3;
	if (ImGui::BeginCombo("##fail_action", tr(failActionKeys[currentAction]).c_str())) {
		for (int i = 0; i < failActionCount; i++) {
			const bool isSelected = (currentAction == i);
			if (ImGui::Selectable(tr(failActionKeys[i]).c_str(), isSelected)) {
				if (currentAction != i) {
					switch (i) {
						case 0: data.failAction = mission_fail_actions::EndProject{};    break;
						case 1: data.failAction = mission_fail_actions::RestartScene{};  break;
						case 2: data.failAction = mission_fail_actions::GotoScene{};     break;
						default: break;
					}
					currentAction = i;
				}
			}
			if (isSelected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	// Render variant-specific payload
	std::visit([&](auto& action) {
		using T = std::decay_t<decltype(action)>;
		if constexpr (std::is_same_v<T, mission_fail_actions::GotoScene>) {
			// Scene combo (mirrors goto_scene.h:31-42)
			const auto& scenes = ScenesManager::instance().getScenesInfo();
			std::string scenePreview;
			for (const auto& scene : scenes) {
				if (scene.id == action.sceneId) {
					scenePreview = scene.name;
					break;
				}
			}
			ImGui::Text(tr("fail_action_goto_scene_target").c_str());
			ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.45f);
			ImGui::SetNextItemWidth(-1.f);
			if (ImGui::BeginCombo("##fail_action_goto_scene", scenePreview.empty() ? "..." : scenePreview.c_str())) {
				for (const auto& scene : scenes) {
					bool selected = scene.id == action.sceneId;
					if (ImGui::Selectable(scene.name.c_str(), selected)) {
						if (action.sceneId != scene.id) {
							action.sceneId = scene.id;
							action.objectiveId = std::nullopt; // reset objective when scene changes
						}
					}
					if (selected) {
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}

			// Objective combo — disabled when sceneId is empty.
			// Heavy work (list build / JSON read) runs ONLY inside BeginCombo (once per open).
			// Preview lookup is cheap: in-memory walk for current scene; cached lookup for others.
			ImGui::BeginDisabled(action.sceneId.empty());
			ImGui::Text(tr("fail_action_goto_scene_objective").c_str());
			ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.45f);
			ImGui::SetNextItemWidth(-1.f);

			// Cache of {sceneId → [(uuid, "i: name")]} for non-current scenes — populated on combo open.
			static std::unordered_map<std::string, std::vector<std::pair<std::string, std::string>>> sObjectivesCache;

			auto findObjectiveLabel = [&](const std::string& sceneId, const std::string& uuid) -> std::string {
				auto& currentScene = ScenesManager::instance().getUnsafeCurrentScene();
				if (currentScene.info.id == sceneId) {
					for (int i = 0; i < static_cast<int>(currentScene.objectives.data.size()); ++i) {
						const auto& obj = currentScene.objectives.data[i];
						if (uuids::to_string(obj.id) == uuid) {
							return fmt::format("{}: {}", i + 1, obj.name);
						}
					}
				} else if (auto it = sObjectivesCache.find(sceneId); it != sObjectivesCache.end()) {
					for (const auto& [u, name] : it->second) {
						if (u == uuid) return name;
					}
				}
				return uuid; // fallback until user opens the combo at least once
			};

			std::string objPreview = action.objectiveId.has_value()
				? findObjectiveLabel(action.sceneId, *action.objectiveId)
				: tr("fail_action_first_objective");

			if (ImGui::BeginCombo("##fail_action_goto_scene_objective", objPreview.c_str())) {
				// Build the full list lazily — this branch runs at most once per combo open.
				std::vector<std::pair<std::string, std::string>> sceneObjectives;
				auto& currentScene = ScenesManager::instance().getUnsafeCurrentScene();
				if (currentScene.info.id == action.sceneId) {
					for (int i = 0; i < static_cast<int>(currentScene.objectives.data.size()); ++i) {
						const auto& obj = currentScene.objectives.data[i];
						sceneObjectives.emplace_back(uuids::to_string(obj.id),
						                             fmt::format("{}: {}", i + 1, obj.name));
					}
				} else {
					const auto& scenesInfo = ScenesManager::instance().getScenesInfo();
					auto it = std::ranges::find_if(scenesInfo, [&](const SceneInfo& si) {
						return si.id == action.sceneId;
					});
					if (it != scenesInfo.end()) {
						std::string path = projectPath("scenes") + "/" + it->id + ".json";
						std::ifstream file(path);
						if (file.is_open()) {
							try {
								nlohmann::json jsonData;
								file >> jsonData;
								// ObjectivesContainer serializes as a top-level JSON array (see objectives_container.cpp).
								if (jsonData.contains("objectives") && jsonData["objectives"].is_array()) {
									int idx = 0;
									for (const auto& objJson : jsonData["objectives"]) {
										sceneObjectives.emplace_back(
											objJson.value("id", ""),
											fmt::format("{}: {}", idx + 1, objJson.value("name", "")));
										++idx;
									}
								}
							} catch (const std::exception&) {
								// silently ignore parse errors in the editor
							}
						}
					}
					sObjectivesCache[action.sceneId] = sceneObjectives; // refresh cache for preview lookup
				}

				// Sentinel: "first objective" → objectiveId = nullopt
				bool sentinelSelected = !action.objectiveId.has_value();
				if (ImGui::Selectable(tr("fail_action_first_objective").c_str(), sentinelSelected)) {
					action.objectiveId = std::nullopt;
				}
				if (sentinelSelected) {
					ImGui::SetItemDefaultFocus();
				}
				for (const auto& [uuid, name] : sceneObjectives) {
					bool objSelected = action.objectiveId.has_value() && *action.objectiveId == uuid;
					if (ImGui::Selectable(name.c_str(), objSelected)) {
						action.objectiveId = uuid;
					}
					if (objSelected) {
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}
			ImGui::EndDisabled();
		}
	}, data.failAction);

	// Fade out checkbox — always rendered, bound to the active alternative's fadeOut field
	std::visit([&](auto& action) {
		ImGui::Text(tr("fade_out").c_str());
		ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.45f);
		ImGui::Checkbox("##fade_out", &action.fadeOut);
		if (ImGui::IsItemHovered()) {
			ImGui::SetTooltip("%s", tr("fade_out_tooltip").c_str());
		}
	}, data.failAction);
}

inline ktwait execute(Data& data) {
	// Guard: nested mission_start while mission is already active is an authoring error.
	if (ProjectPlayer::instance().isMissionActive()) {
		auto& sc = ScenesManager::instance().getUnsafeCurrentScene();
		int idx = ProjectPlayer::instance().getCurrentObjectiveIndex();
		Notifications::error(
			tr("nested_error.title"),
			fmt::format(fmt::runtime(tr("nested_error.body")), sc.info.name, idx));
		LDYOM_ERROR("mission_start called while mission already active (scene='{}', objective_index={})",
		            sc.info.name, idx);
		ProjectPlayer::instance().stopCurrentProject();
		co_return;
	}

	// Cache titleText → gameTitleText (utf8 → cp1251 + gxt). Always re-derive so that clearing
	// titleText between runs drops the stale banner instead of re-displaying the previous one.
	data.gameTitleText = data.titleText;
	if (!data.gameTitleText.empty()) {
		data.gameTitleText = utf8_to_cp1251(data.gameTitleText);
		gxt_encode(data.gameTitleText);
	}

	// Cache failText → local gxt string (only used when mode == Custom); pass into setMissionMode
	std::string failTextGxt;
	if (data.failTextMode == 1 && !data.failText.empty()) {
		failTextGxt = utf8_to_cp1251(data.failText);
		gxt_encode(failTextGxt);
	}
	ProjectPlayer::instance().setMissionMode(data.failTextMode, failTextGxt, data.failAction);

	// Flip the GTA mission flag
	CTheScripts::OnAMissionFlag = 1;

	// Fade-in control: mission_start owns its fade-in (project_player.cpp skips the pre-objective
	// fade for core.mission_start so fadeIn can drive it). When faded, wait for the fade to complete
	// before showing the banner so its duration isn't eaten by the transition.
	if (ProjectPlayer::instance().isFaded()) {
		if (data.fadeIn) {
			plugin::Command<plugin::Commands::DO_FADE>(ProjectPlayer::MISSION_FADE_TIME_MS, 1);
			co_await(ProjectPlayer::MISSION_FADE_TIME_MS + 100);
		} else {
			plugin::Command<plugin::Commands::DO_FADE>(0, 1); // instant reveal
		}
		ProjectPlayer::instance().setFaded(false);
	}

	// Show the mission title banner bottom-right
	if (!data.gameTitleText.empty()) {
		CMessages::AddBigMessage(data.gameTitleText.data(), static_cast<int>(data.titleTime * 1000), STYLE_BOTTOM_RIGHT);
	}
}

inline Objective make() {
	return Objective(TYPE, _("objectives." + std::string(TYPE) + ".name"), Data{},
	                 std::function<void(Data&)>(renderEditor), std::function<ktwait(Data&)>(execute));
}

inline ObjectiveBuilderData builder() { return ObjectiveBuilderData{.type = TYPE, .category = "", .builder = make}; }

} // namespace objectives::mission_start
