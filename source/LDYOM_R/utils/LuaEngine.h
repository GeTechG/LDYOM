#pragma once
#define SOL_LUAJIT 1

#include <sol.hpp>

class LuaEngine
{
private:
	sol::state lua;

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

};

