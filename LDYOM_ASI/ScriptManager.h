#pragma once

#include <vector>
#include <lua.hpp>
#define SOL_ALL_SAFETIES_ON 1
#include <extensions/ScriptCommands.h>
#include <sol/sol.hpp>

#include "libs/ini.h"

class String
{
private:
	std::string value;
public:

	String(std::string& str)
	{
		this->value = str;
	}
	~String() = default;

	const char* getChars()
	{
		return value.c_str();
	}
};

class ScriptManager
{
public:
	static std::vector<std::pair<bool,sol::state&>> lua_scripts;
	
	static void loadScripts();

	static bool ScriptManager::checkProtected(sol::protected_function_result& result);
};

