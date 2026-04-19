#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS
#include "objective.h"
#include <CMessages.h>
#include <CTheScripts.h>
#include <blip_color_utils.h>
#include <common.h>
#include <extensions/ScriptCommands.h>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <imgui_widgets/imgui_widgets.h>
#include <ktcoro_wait.hpp>
#include <localization.h>
#include <logger.h>
#include <notifications.h>
#include <objectives_manager.h>
#include <project_player.h>
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
		auto ut = lua_state.new_usertype<Data>("ObjectiveMissionStartData");
		// failAction is intentionally NOT exposed to Lua in v1 — sol2 requires per-variant-alternative
		// usertype registration plus a custom from_lua path. With only one alternative (EndProject)
		// the work is not justified. See design.md "Lua visibility".
		SOL_LUA_FOR_EACH(SOL_LUA_BIND_MEMBER_ACTION, ut, Data, titleText, titleTime, fadeIn, failTextMode, failText);
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

	// TODO: single-variant scaffolding for std::variant<Action> (only EndProject today).
	// When a 2nd alternative is added, derive currentAction from data.failAction.index() (or std::visit) and write the matching alternative back on selection — otherwise the user's choice is lost every frame.
	int currentAction = 0;
	ImGui::Text(tr("fail_action").c_str());
	ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	constexpr int failActionCount = 1;
	if (ImGui::BeginCombo("##fail_action", tr(fmt::format("fail_actions.{}", currentAction)).c_str())) {
		for (int i = 0; i < failActionCount; i++) {
			const bool isSelected = (currentAction == i);
			if (ImGui::Selectable(tr(fmt::format("fail_actions.{}", i)).c_str(), isSelected)) {
				currentAction = i;
				data.failAction = mission_fail_actions::EndProject{};
			}
			if (isSelected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
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
