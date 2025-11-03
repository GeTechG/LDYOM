#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS
#include "objective.h"
#include <CMessages.h>
#include <CText.h>
#include <blip_color_utils.h>
#include <common.h>
#include <extensions/ScriptCommands.h>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <ktcoro_wait.hpp>
#include <localization.h>
#include <objectives_manager.h>
#include <project_player.h>
#include <string_utils.h>
#include <utils/imgui_configurate.h>
#include <utils/timer_service.h>

namespace objectives::timeout_obj {
constexpr const char* TYPE = "core.timeout";

struct Data {
	int timeSeconds = 5;
	bool showTimer = false;
	std::string text;

	std::string gameText;

	static void sol_lua_register(sol::state_view lua_state) {
		auto ut = lua_state.new_usertype<Data>("ObjectiveTimeoutData");
		SOL_LUA_FOR_EACH(SOL_LUA_BIND_MEMBER_ACTION, ut, Data, timeSeconds, showTimer, text);
	}
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Data, timeSeconds, showTimer, text);
};

std::string tr(const std::string& key) { return _(fmt::format("objectives.{}.{}", std::string(TYPE), key)); }

void renderEditor(Data& data) {
	ImGui::Dummy(ImVec2((SCL_PX).x * 300.f, 0.f));
	const auto availableWidth = ImGui::GetContentRegionAvail().x;

	// Duration
	ImGui::Text(tr("duration").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	int minutes = data.timeSeconds / 60;
	int seconds = data.timeSeconds % 60;
	std::string formatStr = fmt::format("{:02d}:{:02d}", minutes, seconds);
	ImGui::DragInt("##time", &data.timeSeconds, 1.0f, 1, 3600, formatStr.c_str());

	// Show Timer
	ImGui::Text(tr("show_timer").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::Checkbox("##show_timer", &data.showTimer);

	// Text
	ImGui::Text(_("text").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::InputText("##text", &data.text);
}

ktwait execute(Data& data) {
	auto& timerService = TimerService::instance();

	// Display text if provided
	if (!data.text.empty()) {
		auto cp1251Text = utf8_to_cp1251(data.text);
		gxt_encode(cp1251Text);
		data.gameText = cp1251Text;
		CMessages::AddMessage(const_cast<char*>(data.gameText.c_str()), static_cast<unsigned>(data.timeSeconds * 1000),
		                      0, false);
	}

	// Show countdown timer if requested (visual only, doesn't fail mission)
	if (data.showTimer) {
		int timeMs = data.timeSeconds * 1000;
		timerService.addTimer(TheText.Get("RTIME"), true, timeMs);
	}

	// Wait for the duration
	co_await (data.timeSeconds * 1000);

	// Remove timer if it was shown
	if (data.showTimer) {
		timerService.removeTimer();
	}
}

Objective make() {
	return Objective(TYPE, _("objectives." + std::string(TYPE) + ".name"), Data{},
	                 std::function<void(Data&)>(renderEditor), std::function<ktwait(Data&)>(execute));
}

ObjectiveBuilderData builder() { return ObjectiveBuilderData{.type = TYPE, .category = "", .builder = make}; }
} // namespace objectives::timeout_obj
