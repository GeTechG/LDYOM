#pragma once

#include <memory>
#include <mutex>
#include <shared_mutex>
#include <sol/sol.hpp>
#include <string>

class LuaManager {
  public:
	static LuaManager& instance();

	void initialize();

	void shutdown();

	bool executeString(const std::string& code, std::optional<sol::environment> env);

	bool executeFile(const std::string& filepath, std::optional<sol::environment> env);
	bool executeFileRaw(const std::string& filepath, std::optional<sol::environment> env);

	class StateGuard {
	  public:
		StateGuard(sol::state& state, std::shared_mutex& mutex)
			: state_(state),
			  lock_(mutex) {}

		sol::state& get() { return state_; }

	  private:
		sol::state& state_;
		std::unique_lock<std::shared_mutex> lock_;
	};

	StateGuard getState() { return StateGuard(lua, stateMutex); }

	template <typename F> void registerFunction(const std::string& name, F&& func) {
		std::unique_lock lock(stateMutex);
		lua.set_function(name, std::forward<F>(func));
	}

	template <typename T> auto registerType(const std::string& name) {
		std::unique_lock lock(stateMutex);
		return lua.new_usertype<T>(name);
	}

  private:
	LuaManager() = default;
	~LuaManager() = default;
	LuaManager(const LuaManager&) = delete;
	LuaManager& operator=(const LuaManager&) = delete;

	sol::state lua;
	mutable std::shared_mutex stateMutex;
};