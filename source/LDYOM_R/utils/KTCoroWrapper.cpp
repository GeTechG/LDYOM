#include <sol.hpp>

#include <windows.h>
#include "imgui_notify.h"
#include "ktcoro_wait.hpp"
#include "LuaEngine.h"
#include "easylogging/easylogging++.h"

void ktcoroWrapper(sol::state &state) {
	state.new_usertype<ktcoro_tasklist>("ktcoro_tasklist", sol::no_constructor,
	                                    "add_task", [](ktcoro_tasklist &tasklist, sol::protected_function func) {
		                                    tasklist.add_task([](sol::protected_function func) -> ktwait {
			                                    auto &state_ = LuaEngine::getInstance().getLuaState();
			                                    sol::thread runner = sol::thread::create(state_);
			                                    sol::state_view runnerstate = runner.state();
			                                    auto coroutine = sol::coroutine(runnerstate, func);
			                                    while (coroutine) {
				                                    auto result = coroutine();
				                                    if (!result.valid()) {
					                                    LuaEngine::errorHandler(result);
					                                    break;
				                                    }
				                                    int time = 1;
				                                    if (result.return_count() > 0 && result.get_type(0) ==
					                                    sol::type::number) {
					                                    time = result.get<int>(0);
				                                    }
				                                    co_await std::chrono::milliseconds(time);
			                                    }
		                                    }, func);
	                                    }
	);
}
