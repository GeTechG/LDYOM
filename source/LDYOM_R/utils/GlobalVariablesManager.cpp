#include "GlobalVariablesManager.h"

#include "GlobalVariablesService.h"
#include "LuaEngine.h"
#include "easylogging/easylogging++.h"

sol::table GlobalVariablesManager::getPlayingTable() {
	auto &luaState = LuaEngine::getInstance().getLuaState();
	auto dataTable = luaState["data"].get_or_create<sol::table>();
	auto playingTable = dataTable["playing"].get_or_create<sol::table>();
	return playingTable;
}

void GlobalVariablesManager::initVariables() {
	auto &luaState = LuaEngine::getInstance().getLuaState();
	auto globalVariables = luaState.create_table();
	for (int i = 0; i < GlobalVariablesService::getInstance().getSize(); ++i) {
		if (auto [result, var] = GlobalVariablesService::getInstance().get(i); result == 0) {
			switch (var.value.getType()) {
				case GlobalVariableType::Float: {
					globalVariables[var.uuid] = *static_cast<float*>(var.value.value);
					break;
				}
				case GlobalVariableType::Int: {
					globalVariables[var.uuid] = *static_cast<int*>(var.value.value);
					CLOG(DEBUG, "ldyom") << "Global variable " << globalVariables[var.uuid].get<int>();
					break;
				}
				case GlobalVariableType::String: {
					globalVariables[var.uuid] = *static_cast<std::string*>(var.value.value);
					break;
				}
				case GlobalVariableType::Bool: {
					globalVariables[var.uuid] = *static_cast<bool*>(var.value.value);
					break;
				}
			}
		}
	}
	getPlayingTable()["globalVariables"] = globalVariables;
}

void GlobalVariablesManager::resetVariables() {
	getPlayingTable()["globalVariables"] = sol::lua_nil;
}

GlobalVariableView::Value GlobalVariablesManager::getGlobalVariable(const std::string &uuid) {
	auto playingTable = getPlayingTable();
	auto globalVariables = playingTable["globalVariables"].get<sol::table>();
	auto [result, var] = GlobalVariablesService::getInstance().get(uuid);
	GlobalVariableView::Value value{var.value.getType()};
	switch (var.value.getType()) {
		case GlobalVariableType::Float: {
			*static_cast<float*>(value.value) = globalVariables[uuid].get<float>();
			break;
		}
		case GlobalVariableType::Int: {
			*static_cast<int*>(value.value) = globalVariables[uuid].get<int>();
			break;
		}
		case GlobalVariableType::String: {
			*static_cast<std::string*>(value.value) = globalVariables[uuid].get<std::string>();
			break;
		}
		case GlobalVariableType::Bool: {
			*static_cast<bool*>(value.value) = globalVariables[uuid].get<bool>();
			break;
		}
	}
	return std::move(value);
}
