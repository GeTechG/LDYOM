#include <sol.hpp>

#include <windows.h>
#include "imgui_notify.h"
#include "ktcoro_wait.hpp"
#include "LuaEngine.h"
#include "easylogging/easylogging++.h"

void ktcoroWrapper(sol::state &state) {
	state.new_usertype<ktcoro_tasklist>("ktcoro_tasklist", sol::no_constructor,
	                                    "add_task", [](ktcoro_tasklist &tasklist, sol::function func) {
		                                    tasklist.add_task([](sol::function func) -> ktwait {
			                                    sol::thread thread = LuaEngine::getInstance().getLuaState()["coroutine"]
			                                    [
				                                    "create"](func);
			                                    while (thread) {
				                                    auto &state = LuaEngine::getInstance().getLuaState();
				                                    std::string status = state["coroutine"]["status"](
					                                    thread);
				                                    if (status == "dead") {
					                                    break;
				                                    }
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
				                                    int time = 1;
				                                    if (result.return_count() > 1 && result.get_type(1) ==
					                                    sol::type::number) {
					                                    time = result.get<int>(1);
				                                    }
				                                    co_await std::chrono::milliseconds(time);
			                                    }
		                                    }, func);
	                                    }
	);
}
