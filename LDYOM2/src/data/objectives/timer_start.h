#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS
#include "objective.h"
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

namespace objectives::timer_start {
constexpr const char* TYPE = "core.timer_start";

struct Data {
	static void sol_lua_register(sol::state_view lua_state) {
		auto ut = lua_state.new_usertype<Data>("ObjectiveTimerStartData");
	}

	friend void to_json(nlohmann::json& j, const Data& data) { j = nlohmann::json::object(); }

	friend void from_json(const nlohmann::json& j, Data& data) {
		// No properties to deserialize
	}
};

std::string tr(const std::string& key) { return _(fmt::format("objectives.{}.{}", std::string(TYPE), key)); }

void renderEditor(Data& data) {
	ImGui::Dummy(ImVec2((SCL_PX).x * 300.f, 0.f));
	ImGui::TextWrapped("%s", tr("info").c_str());
}

ktwait execute(Data& data) {
	auto& timerService = TimerService::instance();

	if (!timerService.isTimerActive()) {
		// Start a count-up timer (type 0 = count up, shows elapsed time)
		// Timer starts at 0 and counts upward
		timerService.addTimer(TheText.Get("BB_19"), false, 0);
	}

	co_return;
}

Objective make() {
	return Objective(TYPE, _("objectives." + std::string(TYPE) + ".name"), Data{},
	                 std::function<void(Data&)>(renderEditor), std::function<ktwait(Data&)>(execute));
}

ObjectiveBuilderData builder() { return ObjectiveBuilderData{.type = TYPE, .category = "", .builder = make}; }
} // namespace objectives::timer_start
