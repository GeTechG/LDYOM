#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS
#include "objective.h"
#include <CMessages.h>
#include <CTheScripts.h>
#include <common.h>
#include <extensions/ScriptCommands.h>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <imgui_widgets/imgui_widgets.h>
#include <ktcoro_wait.hpp>
#include <localization.h>
#include <objectives_manager.h>
#include <project_player.h>
#include <string_utils.h>
#include <utils/imgui_configurate.h>

namespace objectives::mission_end {
constexpr const char* TYPE = "core.mission_end";

struct Data {
	int endTextMode = 0; // 0 = Default (M_PASS), 1 = With money (M_PASSD), 2 = Custom
	std::string endText; // only used when endTextMode == 2
	int endMoney = 0;    // only used when endTextMode == 1 (displayed in the banner; does NOT grant money — chain with money_add)
	float endTime = 1.f;
	bool fadeOut = false;
	bool endTune = false;
	std::string gameEndText; // cached gxt for the custom banner; not persisted

	static void sol_lua_register(sol::state_view lua_state) {
		auto ut = lua_state.new_usertype<Data>("ObjectiveMissionEndData");
		SOL_LUA_FOR_EACH(SOL_LUA_BIND_MEMBER_ACTION, ut, Data, endTextMode, endText, endMoney, endTime, fadeOut, endTune);
	}
	NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Data, endTextMode, endText, endMoney, endTime, fadeOut, endTune);
};

inline std::string tr(const std::string& key) { return _(fmt::format("objectives.{}.{}", std::string(TYPE), key)); }

inline void renderEditor(Data& data) {
	ImGui::Dummy(ImVec2((SCL_PX).x * 300.f, 0.f));

	ImGui::Text(tr("end_text_mode").c_str());
	ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	constexpr int endModeCount = 3;
	if (ImGui::BeginCombo("##end_text_mode", tr(fmt::format("end_text_modes.{}", data.endTextMode)).c_str())) {
		for (int i = 0; i < endModeCount; i++) {
			const bool isSelected = (data.endTextMode == i);
			if (ImGui::Selectable(tr(fmt::format("end_text_modes.{}", i)).c_str(), isSelected)) {
				data.endTextMode = i;
			}
			if (isSelected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	if (data.endTextMode == 1) {
		ImGui::Text(tr("end_money").c_str());
		ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.45f);
		ImGui::SetNextItemWidth(-1.f);
		ImGui::InputInt("##end_money", &data.endMoney);
	} else if (data.endTextMode == 2) {
		ImGui::Text(tr("end_text").c_str());
		ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.45f);
		ImGui::SetNextItemWidth(-1.f);
		ImGui::InputTextWithPopup("##end_text", &data.endText);
	}

	ImGui::Text(tr("end_time").c_str());
	ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::DragFloat("##end_time", &data.endTime, 0.1f, 0.f, 10.f, "%.1f");

	ImGui::Text(tr("fade_out").c_str());
	ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.45f);
	ImGui::Checkbox("##fade_out", &data.fadeOut);

	ImGui::Text(tr("end_tune").c_str());
	ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.45f);
	ImGui::Checkbox("##end_tune", &data.endTune);
}

inline ktwait execute(Data& data) {
	// Guard: mission_end is a no-op if no mission is currently active (orphan-end).
	if (!ProjectPlayer::instance().isMissionActive()) {
		co_return;
	}

	// Cache endText → gameEndText for Custom mode. Always re-derive so that clearing endText
	// between runs drops the stale banner instead of re-displaying the previous one.
	if (data.endTextMode == 2) {
		data.gameEndText = data.endText;
		if (!data.gameEndText.empty()) {
			data.gameEndText = utf8_to_cp1251(data.gameEndText);
			gxt_encode(data.gameEndText);
		}
	}

	// Optional fade-out: mirror cutscene.h:85-87 pattern
	if (data.fadeOut && !ProjectPlayer::instance().isFaded()) {
		plugin::Command<plugin::Commands::DO_FADE>(ProjectPlayer::MISSION_FADE_TIME_MS, 0);
		co_await(ProjectPlayer::MISSION_FADE_TIME_MS + 100);
		ProjectPlayer::instance().setFaded(true);
	}

	// Optional end-tune: classic SA mission-passed tune (DYOM uses the same opcode 0394 on mission pass)
	if (data.endTune) {
		plugin::Command<plugin::Commands::PLAY_MISSION_PASSED_TUNE>(1);
	}

	// Show the mission end banner based on the selected mode
	const int endTimeMs = static_cast<int>(data.endTime * 1000);
	switch (data.endTextMode) {
	case 0: // Default: built-in M_PASSD (plain "MISSION PASSED!", opcode style 1 == STYLE_MIDDLE)
		plugin::Command<plugin::Commands::PRINT_BIG>("M_PASSD", endTimeMs, 1);
		break;
	case 1: // With money: M_PASS is the 1-number GXT entry that renders "MISSION PASSED!\n$<amount>"
		plugin::Command<plugin::Commands::PRINT_WITH_NUMBER_BIG>("M_PASS", data.endMoney, endTimeMs, 1);
		break;
	case 2: // Custom author text
		if (!data.gameEndText.empty()) {
			CMessages::AddBigMessage(data.gameEndText.data(), endTimeMs, STYLE_MIDDLE);
		}
		break;
	}

	// Clear the GTA mission flag and mission mode state
	CTheScripts::OnAMissionFlag = 0;
	ProjectPlayer::instance().clearMissionMode();
}

inline Objective make() {
	return Objective(TYPE, _("objectives." + std::string(TYPE) + ".name"), Data{},
	                 std::function<void(Data&)>(renderEditor), std::function<ktwait(Data&)>(execute));
}

inline ObjectiveBuilderData builder() { return ObjectiveBuilderData{.type = TYPE, .category = "", .builder = make}; }

} // namespace objectives::mission_end
