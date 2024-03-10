#include <sol.hpp>

#include <windows.h>
#include "imgui_notify.h"
#include "ktcoro_wait.hpp"
#include "LuaEngine.h"
#include "Tasker.h"

ktwait taskWrapper(sol::function func) {
	auto &state_ = LuaEngine::getInstance().getLuaState();
	sol::thread runner = sol::thread::create(state_);
	sol::state_view runnerstate = runner.state();
	auto coroutine = sol::coroutine(runnerstate, func);
	while (coroutine) {
		int time = 1;
		auto result = coroutine();
		if (!result.valid()) {
			LuaEngine::errorHandler(result);
			ImGui::InsertNotification({
				ImGuiToastType_Error, 1000, "Error, see log"
			});
			break;
		}
		if (result.return_count() > 0 && result.get_type(0) ==
			sol::type::number) {
			time = result.get<int>(0);
		}
		co_await std::chrono::milliseconds(time);
	}
}

void taskerWrapper(sol::state &state) {
	auto ldyomTable = state["ld"].get_or_create<sol::table>();
	ldyomTable["tasker"] = &Tasker::getInstance();
	state.new_usertype<Tasker>("LDTasker",
	                           "addTask", [](Tasker &tasker, std::string name, sol::function func) {
		                           tasker.addTask(name, taskWrapper, func);
	                           },
	                           "removeTask", &Tasker::removeTask
	);
}
