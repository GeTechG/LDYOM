#pragma once
#define SOL_LUAJIT 1

#include <filesystem>
#include <sol.hpp>
#include <boost/signals2.hpp>

class LuaEngine {
private:
	sol::state luaState_;
	boost::signals2::signal<void()> onReset;

	void addScriptDirToLuaPaths(std::string &luaPaths, const std::filesystem::directory_entry &entry);
	void loadScripts(std::string luaPaths);
	void resetState();

	LuaEngine() = default;
	LuaEngine(const LuaEngine &) = delete;
	LuaEngine& operator=(LuaEngine &) = delete;

public:
	static LuaEngine& getInstance() {
		static LuaEngine instance;
		return instance;
	}

	void Init();
	sol::state& getLuaState();
	boost::signals2::signal<void()>& getOnReset();
	void shutdown();
	static sol::protected_function_result errorHandlerCallback(sol::this_state, sol::protected_function_result pfr);
	static void errorHandler(const sol::protected_function_result &pfr);
};
