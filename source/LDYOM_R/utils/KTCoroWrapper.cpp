#include <sol.hpp>

#include <windows.h>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <fmt/core.h>
#include "imgui_notify.h"
#include "ktcoro_wait.hpp"
#include "LuaEngine.h"
#include "easylogging/easylogging++.h"

void ktcoroWrapper(sol::state &state) {
	state.new_usertype<ktcoro_tasklist>("ktcoro_tasklist", sol::no_constructor,
	                                    "add_task", [](ktcoro_tasklist &tasklist, std::string id,
	                                                   sol::protected_function func) {
		                                    boost::uuids::uuid uuid = boost::uuids::random_generator()();
		                                    id = fmt::format("{}-{}", id, to_string(uuid));
		                                    CLOG(INFO, "LDYOM") << "Adding lua task: " << id;
		                                    tasklist.add_task(
			                                    [](std::string id, sol::protected_function func) -> ktwait {
				                                    auto &state_ = LuaEngine::getInstance().getLuaState();
				                                    sol::thread runner = sol::thread::create(state_);
				                                    sol::state_view runnerstate = runner.state();
				                                    auto coroutine = sol::coroutine(runnerstate, func);
				                                    while (coroutine) {
					                                    auto result = coroutine();
					                                    if (!result.valid()) {
						                                    LuaEngine::errorHandler(result);
						                                    const sol::error err = result;
						                                    ImGui::InsertNotification({
							                                    ImGuiToastType_Error, 1000,
							                                    fmt::format("Error: {}", err.what()).c_str()
						                                    });
						                                    break;
					                                    }
					                                    int time = 1;
					                                    if (result.return_count() > 0 && result.get_type(0) ==
						                                    sol::type::number) {
						                                    time = result.get<int>(0);
					                                    }
					                                    co_await std::chrono::milliseconds(time);
				                                    }
				                                    CLOG(INFO, "LDYOM") << "Lua task finished: " << id;
			                                    }, id, func);
	                                    }
	);
}
