#pragma once
#include <sol.hpp>

class FilesystemLuaWrapper {
public:
	static void wrap(sol::state& state);
};
