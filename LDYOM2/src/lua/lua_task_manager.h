#pragma once
#include <chrono>
#include <sol/sol.hpp>
#include <string>
#include <unordered_map>
#include <vector>

class LuaTaskManager {
  public:
	LuaTaskManager() = default;

	void processAll();
	void shutdown();

	// C++ callable (also exposed to Lua)
	bool run(std::string key, sol::function func);
	bool isRunning(const std::string& key);
	bool cancel(const std::string& key);
	bool cancelAll();

	// Pass the manager instance to bind against.
	// tableName controls the name of the Lua global created (default: "tasks").
	static void registerBindings(sol::state_view lua, LuaTaskManager& manager, std::string_view tableName = "tasks");

  private:
	enum class Reason { NextFrame, Sleep, WaitFor };

	struct Task {
		sol::coroutine coro;
		Reason waitReason = Reason::NextFrame;
		std::chrono::steady_clock::time_point wakeTime;
		std::string waitForKey;
		bool completed = false;
	};

	void processResult(Task& task, sol::protected_function_result& result, std::chrono::steady_clock::time_point now);

	std::unordered_map<std::string, Task> m_tasks;
	std::vector<std::pair<std::string, Task>> m_pending;
	bool m_inProcess = false;
};
