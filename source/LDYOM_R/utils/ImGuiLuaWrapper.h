#pragma once
#include <sol.hpp>

class ImGuiLuaWrapper {
public:
	static void wrap(sol::state& state);
};
