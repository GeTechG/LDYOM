#pragma once
#include <sol.hpp>

class LuaWrapper {
public:
	static void wrap(sol::state& state);
};
