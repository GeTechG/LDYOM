#include <sol.hpp>

#include "Settings.h"

void settingsWrapper(sol::state &state) {
	auto ldTable = state["ld"].get<sol::table>();
	auto table = ldTable.create("settings");
	table["getBool"] = [](const char *key) {
		return Settings::getInstance().get<bool>(key);
	};
	table["getInt"] = [](const char *key) {
		return Settings::getInstance().get<int>(key);
	};
	table["getFloat"] = [](const char *key) {
		return Settings::getInstance().get<float>(key);
	};
	table["getString"] = [](const char *key) {
		return Settings::getInstance().get<std::string>(key);
	};
	table["setBool"] = [](const char *key, bool value) {
		Settings::getInstance().set(key, value);
	};
	table["setInt"] = [](const char *key, int value) {
		Settings::getInstance().set(key, value);
	};
	table["setFloat"] = [](const char *key, float value) {
		Settings::getInstance().set(key, value);
	};
	table["setString"] = [](const char *key, std::string value) {
		Settings::getInstance().set(key, value);
	};
	table["save"] = []() {
		Settings::getInstance().Save();
	};
}
