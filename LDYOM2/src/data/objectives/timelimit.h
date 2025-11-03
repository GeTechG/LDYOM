#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS
#include "objective.h"
#include <CMessages.h>
#include <atomic>
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

namespace objectives::timelimit {
constexpr const char* TYPE = "core.timelimit";

enum class Mode {
	Set = 0, // Set new time limit
	Add = 1, // Add time to existing limit
	Stop = 2 // Stop the timer
};

struct Data {
	Mode mode = Mode::Set;
	int timeSeconds = 60; // Time in seconds

	static void sol_lua_register(sol::state_view lua_state) {
		auto ut = lua_state.new_usertype<Data>("ObjectiveTimelimitData");
		SOL_LUA_FOR_EACH(SOL_LUA_BIND_MEMBER_ACTION, ut, Data, mode, timeSeconds);
	}
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Data, mode, timeSeconds);
};

std::string tr(const std::string& key) { return _(fmt::format("objectives.{}.{}", std::string(TYPE), key)); }

void renderEditor(Data& data) {
	ImGui::Dummy(ImVec2((SCL_PX).x * 300.f, 0.f));
	const auto availableWidth = ImGui::GetContentRegionAvail().x;

	// Mode selection
	ImGui::Text(tr("mode_label").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	int modeInt = static_cast<int>(data.mode);
	if (ImGui::BeginCombo("##mode", tr(fmt::format("mode.{}", modeInt)).c_str())) {
		for (int i = 0; i < 3; i++) {
			const bool isSelected = (modeInt == i);
			if (ImGui::Selectable(tr(fmt::format("mode.{}", i)).c_str(), isSelected)) {
				modeInt = i;
				data.mode = static_cast<Mode>(modeInt);
			}
			if (isSelected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	// Time input (only if not stopping)
	if (data.mode != Mode::Stop) {
		ImGui::Text(tr("time_seconds").c_str());
		ImGui::SameLine(availableWidth * 0.45f);
		ImGui::SetNextItemWidth(-1.f);
		// Calculate minutes and seconds for display
		int minutes = data.timeSeconds / 60;
		int seconds = data.timeSeconds % 60;
		std::string formatStr = fmt::format("{:02d}:{:02d}", minutes, seconds);
		ImGui::DragInt("##time", &data.timeSeconds, 1.0f, 1, 3600, formatStr.c_str());
	}
}

ktwait execute(Data& data) {
	auto& timerService = TimerService::instance();
	auto& projectTasklist = ProjectPlayer::instance().projectTasklist;

	switch (data.mode) {
		case Mode::Set:
			{
				if (!timerService.isTimerActive()) {
					// Set new time limit (convert seconds to milliseconds)
					int timeMs = data.timeSeconds * 1000;
					timerService.addTimer(TheText.Get("RTIME"), true, timeMs);
					projectTasklist->add_task([]() -> ktwait {
						while (TimerService::instance().isTimerActive() &&
						       TimerService::instance().getTimerTime() > 0) {
							co_await 100; // Check every 100ms
						}
						// Timer expired - fail the mission
						if (TimerService::instance().isTimerActive() && TimerService::instance().getTimerTime() <= 0) {
							ProjectPlayer::instance().failCurrentProject();
						}
					});
				}
				break;
			}
		case Mode::Add:
			{
				// Add time to existing timer
				if (timerService.isTimerActive()) {
					int currentTime = timerService.getTimerTime();
					int newTime = currentTime + (data.timeSeconds * 1000);
					// Update timer value directly without recreating the display
					timerService.setTimerTime(newTime);
					// Monitor task should already be running and will continue
				}
				break;
			}
		case Mode::Stop:
			{
				// Stop/remove the timer
				timerService.removeTimer();
				// The monitoring task will exit automatically when timer becomes inactive
				// and set monitoringTaskActive to false
				break;
			}
	}

	co_return;
}

Objective make() {
	return Objective(TYPE, _("objectives." + std::string(TYPE) + ".name"), Data{},
	                 std::function<void(Data&)>(renderEditor), std::function<ktwait(Data&)>(execute));
}

ObjectiveBuilderData builder() { return ObjectiveBuilderData{.type = TYPE, .category = "", .builder = make}; }
} // namespace objectives::timelimit
