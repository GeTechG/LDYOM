#include <sol.hpp>

#include <windows.h>
#include "imgui_notify.h"
#include "ktcoro_wait.hpp"
#include "LuaEngine.h"
#include "Tasker.h"
#include "easylogging/easylogging++.h"

ktwait taskWrapper(sol::function func) {
	auto &state = LuaEngine::getInstance().getLuaState();
	sol::thread thread = state["coroutine"]["create"](func);
	while (thread) {
		std::string status = state["coroutine"]["status"](
			thread);
		if (status == "dead") {
			break;
		}
		int time = 1;
		if (status == "suspended") {
			auto result = state["coroutine"]["resume"](thread);
			if (!result.get<bool>(0)) {
				CLOG(ERROR, "lua") << result.get<std::string>(1);
				ImGui::InsertNotification({
					ImGuiToastType_Error, 1000, "Error, see log"
				});
				break;
			}
			if (!result.valid()) {
				LuaEngine::errorHandler(result);
				break;
			}
			if (result.return_count() > 1 && result.get_type(1) ==
				sol::type::number) {
				time = result.get<int>(1);
			}
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
