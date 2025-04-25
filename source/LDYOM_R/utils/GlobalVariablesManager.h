#pragma once
#include <sol.hpp>
#include <string>

#include "GlobalVariablesService.h"

class GlobalVariablesManager {
private:
	sol::table getPlayingTable();

public:
	void initVariables();
	void resetVariables();

	template <typename T>
	void incrementGlobalVariable(const std::string &uuid, T value);
	template <typename T>
	void setGlobalVariable(const std::string &uuid, T value);

	GlobalVariableView::Value getGlobalVariable(const std::string &uuid);
};

template <typename T>
void GlobalVariablesManager::incrementGlobalVariable(const std::string &uuid, T value) {
	auto playingTable = getPlayingTable();
	auto globalVariables = playingTable["globalVariables"].get_or_create<sol::table>();
	const auto var = globalVariables[uuid];
	if (var.get_type() == sol::type::nil) {
		return;
	}
	if (var.get_type() == sol::type::number) {
		globalVariables[uuid] = var.get<T>() + value;
	}
}

template <typename T>
void GlobalVariablesManager::setGlobalVariable(const std::string &uuid, T value) {
	auto playingTable = getPlayingTable();
	auto globalVariables = playingTable["globalVariables"].get_or_create<sol::table>();
	if (const auto var = globalVariables[uuid]; var.get_type() == sol::type::nil) {
		return;
	}
	globalVariables[uuid] = value;
}
