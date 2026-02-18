#pragma once
#include <sol/sol.hpp>

void register_project_player_bindings(sol::state_view lua);
void clear_project_player_lua_callbacks();
