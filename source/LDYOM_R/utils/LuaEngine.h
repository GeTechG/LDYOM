#pragma once
#define SOL_LUAJIT 1

#include <sol.hpp>

class LuaEngine
{
private:
	sol::state luaState_;

	void resetState();

	LuaEngine() = default;
	LuaEngine(const LuaEngine&) = delete;
	LuaEngine& operator=(LuaEngine&) = delete;
public:
	static LuaEngine& getInstance() {
		static LuaEngine instance;
		return instance;
	}

	void Init();
	sol::state& getLuaState();
	static sol::protected_function_result errorHandler(sol::this_state, sol::protected_function_result pfr);
};

