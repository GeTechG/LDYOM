#include "lua_task_manager.h"
#include "utils/logger.h"

LuaTaskManager& LuaTaskManager::instance() {
	static LuaTaskManager inst;
	return inst;
}

void LuaTaskManager::processResult(Task& task, sol::protected_function_result& result,
                                   std::chrono::steady_clock::time_point now) {
	if (!result.valid()) {
		sol::error err = result;
		LDYOM_ERROR("LuaTask error: {}", err.what());
		task.completed = true;
		return;
	}

	auto tag = result.get<sol::optional<std::string>>(0);
	if (!tag) {
		// No yield tag → resume next frame (default)
		task.waitReason = Reason::NextFrame;
		return;
	}

	if (*tag == "sleep") {
		auto ms_opt = result.get<sol::optional<double>>(1);
		double ms = ms_opt ? *ms_opt : 0.0;
		task.waitReason = Reason::Sleep;
		task.wakeTime = now + std::chrono::milliseconds(static_cast<int64_t>(ms));
	} else if (*tag == "wait") {
		auto key_opt = result.get<sol::optional<std::string>>(1);
		task.waitReason = Reason::WaitFor;
		task.waitForKey = key_opt ? *key_opt : "";
	} else {
		task.waitReason = Reason::NextFrame;
	}
}

void LuaTaskManager::processAll() {
	auto now = std::chrono::steady_clock::now();
	m_inProcess = true;

	for (auto& [key, task] : m_tasks) {
		if (task.completed)
			continue;

		bool shouldResume = false;
		switch (task.waitReason) {
		case Reason::NextFrame:
			shouldResume = true;
			break;
		case Reason::Sleep:
			shouldResume = (now >= task.wakeTime);
			break;
		case Reason::WaitFor: {
			auto it = m_tasks.find(task.waitForKey);
			if (it == m_tasks.end()) {
				// Key is not in active tasks — check if it was just submitted (pending)
				bool inPending = false;
				for (const auto& p : m_pending) {
					if (p.first == task.waitForKey) {
						inPending = true;
						break;
					}
				}
				shouldResume = !inPending; // not pending → already gone → done
			} else {
				shouldResume = it->second.completed;
			}
			break;
		}
		}

		if (!shouldResume)
			continue;

		// Reset to NextFrame before resuming; processResult may override
		task.waitReason = Reason::NextFrame;

		auto result = task.coro();

		if (!task.coro.runnable()) {
			task.completed = true;
		} else {
			processResult(task, result, now);
		}
	}

	m_inProcess = false;

	// Flush tasks that were submitted mid-frame
	for (auto& [k, t] : m_pending) {
		m_tasks.emplace(std::move(k), std::move(t));
	}
	m_pending.clear();

	// Remove completed tasks
	std::erase_if(m_tasks, [](const auto& p) { return p.second.completed; });
}

void LuaTaskManager::shutdown() {
	m_tasks.clear();
	m_pending.clear();
	m_inProcess = false;
}

bool LuaTaskManager::run(std::string key, sol::function func) {
	// Reject duplicate keys in active tasks
	if (m_tasks.find(key) != m_tasks.end())
		return false;

	// Reject duplicate keys in pending tasks
	for (const auto& p : m_pending) {
		if (p.first == key)
			return false;
	}

	Task task;
	task.coro = sol::coroutine(func);

	auto now = std::chrono::steady_clock::now();

	// First resume — the Lua wrapper closure captures any caller-supplied args
	auto result = task.coro();

	if (!task.coro.runnable()) {
		task.completed = true;
	} else {
		processResult(task, result, now);
	}

	if (m_inProcess) {
		m_pending.push_back({std::move(key), std::move(task)});
	} else {
		m_tasks.emplace(std::move(key), std::move(task));
	}

	return true;
}

bool LuaTaskManager::isRunning(const std::string& key) {
	auto it = m_tasks.find(key);
	if (it != m_tasks.end() && !it->second.completed)
		return true;
	for (const auto& p : m_pending) {
		if (p.first == key)
			return true;
	}
	return false;
}

bool LuaTaskManager::cancel(const std::string& key) {
	auto it = m_tasks.find(key);
	if (it != m_tasks.end()) {
		it->second.completed = true;
		return true;
	}
	for (auto& p : m_pending) {
		if (p.first == key) {
			p.second.completed = true;
			return true;
		}
	}
	return false;
}

void LuaTaskManager::registerBindings(sol::state_view lua) {
	// Raw internal table — not exposed directly to addon authors
	sol::table raw = lua.create_table("_ltm_raw");

	raw["_run"] = [](std::string key, sol::function func) -> bool {
		return LuaTaskManager::instance().run(std::move(key), func);
	};
	raw["_is_running"] = [](const std::string& key) -> bool {
		return LuaTaskManager::instance().isRunning(key);
	};
	raw["_cancel"] = [](const std::string& key) -> bool {
		return LuaTaskManager::instance().cancel(key);
	};

	// Build the public `tasks` table.
	// The wrapper captures variadic args in a closure so that `_run` always
	// receives a zero-argument function and args are forwarded safely.
	lua.script(R"(
		local _r = _ltm_raw
		tasks = {}

		tasks.run = function(k, f, ...)
			local args = {...}
			return _r._run(k, function() return f(table.unpack(args)) end)
		end

		tasks.is_running = _r._is_running
		tasks.cancel     = _r._cancel

		tasks.sleep    = function(ms)  coroutine.yield("sleep", ms)  end
		tasks.wait_for = function(key) coroutine.yield("wait",  key) end
		tasks.yield    = function()    coroutine.yield()              end

		_ltm_raw = nil
	)");
}
