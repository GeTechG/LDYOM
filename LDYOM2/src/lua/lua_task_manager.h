#pragma once
#include <chrono>
#include <sol/sol.hpp>
#include <string>
#include <unordered_map>
#include <vector>

class LuaTaskManager {
  public:
	static LuaTaskManager& instance();

	void processAll();
	void shutdown();

	// C++ callable (also exposed to Lua)
	bool run(std::string key, sol::function func);
	bool isRunning(const std::string& key);
	bool cancel(const std::string& key);
	bool cancelAll();

	static void registerBindings(sol::state_view lua);

  private:
	LuaTaskManager() = default;

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
