#include <chrono>
#include <sol.hpp>

#include "ktcoro_wait.hpp"
#include "easylogging/easylogging++.h"

void asyncWrapper(sol::state &state) {
	auto tasker = state.create_table("Tasker");
	tasker.set_function("addTask", [](ktcoro_tasklist &tasklist, const sol::coroutine &func) {
		tasklist.add_task([](sol::coroutine func) -> ktwait {
			while (func) {
				auto result = func();
				if (!result.valid()) {
					sol::error err = result;
					CLOG(ERROR, "lua") << err.what();
					break;
				}
				if (result.return_count() > 0 && result.get_type() == sol::type::number)
					co_await std::chrono::milliseconds(result.get<int>());
			}
		}, func);
	});
}
