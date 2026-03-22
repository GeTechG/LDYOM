#include "lua_task_manager.h"
#include "utils/logger.h"

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
			case Reason::NextFrame: shouldResume = true; break;
			case Reason::Sleep: shouldResume = (now >= task.wakeTime); break;
			case Reason::WaitFor:
				{
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
			if (!result.valid()) {
				sol::error err = result;
				LDYOM_ERROR("LuaTask error [{}]: {}", key, err.what());
			}
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

	if (!m_mainL)
		return false;

	Task task;
	// Create an independent Lua thread so the coroutine runs on its own stack.
	// sol::coroutine(func) copies lua_state() from func, which — when called
	// from inside a running coroutine — is the *calling* coroutine's thread.
	// lua_resume() would then resume the CALLER instead of a new coroutine.
	// Fix: create a fresh sol::thread and build the coroutine from its state,
	// so m_L points to the new thread and lua_resume targets the right stack.
	task.thread = sol::thread::create(m_mainL);
	task.coro   = sol::coroutine(task.thread.state().lua_state(), func);

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
		LDYOM_INFO("Cancelled LuaTask [{}]", key);
		return true;
	}
	for (auto& p : m_pending) {
		if (p.first == key) {
			p.second.completed = true;
			LDYOM_INFO("Cancelled pending LuaTask [{}]", key);
			return true;
		}
	}
	return false;
}

bool LuaTaskManager::cancelAll() {
	for (auto& [_, task] : m_tasks) {
		LDYOM_INFO("Cancelled LuaTask [{}]", _);
		task.completed = true;
	}
	for (auto& [_, task] : m_pending) {
		LDYOM_INFO("Cancelled pending LuaTask [{}]", _);
		task.completed = true;
	}
	return true;
}

void LuaTaskManager::registerBindings(sol::state_view lua, LuaTaskManager& manager, std::string_view tableName) {
	// Store the main Lua state so run() can create threads on it instead of
	// on whatever coroutine thread happens to be calling run().
	manager.m_mainL = lua.lua_state();

	// Raw internal table — not exposed directly to addon authors
	const std::string rawKey = std::string("_ltm_raw_") + std::string(tableName);
	sol::table raw = lua.create_table(rawKey);

	raw["_run"] = [&manager](std::string key, sol::function func) -> bool {
		return manager.run(std::move(key), func);
	};
	raw["_is_running"] = [&manager](const std::string& key) -> bool { return manager.isRunning(key); };
	raw["_cancel"] = [&manager](const std::string& key) -> bool { return manager.cancel(key); };

	// Build the public table under `tableName`.
	// The wrapper captures variadic args in a closure so that `_run` always
	// receives a zero-argument function and args are forwarded safely.
	const std::string script = std::string(R"(
		local _raw_key = ')") + std::string(tableName) + R"('
		local _r = _G["_ltm_raw_" .. _raw_key]
		local t = {}

		t.run = function(k, f, ...)
			local args = {...}
			return _r._run(k, function() return f(unpack(args)) end)
		end

		t.is_running = _r._is_running
		t.cancel     = _r._cancel

		t.sleep    = function(ms)  coroutine.yield("sleep", ms)  end
		t.wait_for = function(key) coroutine.yield("wait",  key) end
		t.yield    = function()    coroutine.yield()              end

		_G[_raw_key] = t
		_G["_ltm_raw_" .. _raw_key] = nil
	)";
	lua.script(script);
}
