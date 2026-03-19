#include "global_vars_service.h"
#include "data/global_variable.h"
#include "projects_manager.h"
#include <logger.h>

GlobalVarsService& GlobalVarsService::instance() {
	static GlobalVarsService inst;
	return inst;
}

void GlobalVarsService::initialize(sol::state_view lua) {
	m_gvTable = lua.create_named_table("global_vars");
	m_gvTable["_store"] = lua.create_table();

	lua.script(R"(
		function global_vars.get(name)    return global_vars._store[name] end
		function global_vars.set(name, v) global_vars._store[name] = v   end
		function global_vars.exists(name) return global_vars._store[name] ~= nil end
	)");

	m_gvTable.set_function("reset", []() { GlobalVarsService::instance().reset(); });
	m_gvTable.set_function("getDefault",
	                       [](const std::string& name) { return GlobalVarsService::instance().getDefault(name); });
	m_gvTable.set_function("setDefault", [](const std::string& name, sol::object value) {
		GlobalVarsService::instance().setDefault(name, value);
	});
	m_gvTable.set_function("list", [](sol::this_state s) -> sol::table {
		sol::state_view lua(s);
		sol::table result = lua.create_table();
		auto proj = ProjectsManager::instance().getCurrentProject();
		if (!proj.has_value())
			return result;
		int i = 1;
		for (const auto& var : proj.value()->globalVars) {
			sol::table entry = lua.create_table();
			entry["name"] = var.name;
			switch (var.type()) {
				case GlobalVarType::Number: entry["type"] = "number"; break;
				case GlobalVarType::String: entry["type"] = "string"; break;
				case GlobalVarType::Boolean: entry["type"] = "bool"; break;
			}
			result[i++] = entry;
		}
		return result;
	});
}

void GlobalVarsService::reset() {
	if (!m_gvTable.valid())
		return;

	sol::state_view lua = m_gvTable.lua_state();
	sol::table newStore = lua.create_table();

	auto proj = ProjectsManager::instance().getCurrentProject();
	if (proj.has_value()) {
		for (const auto& var : proj.value()->globalVars) {
			std::visit([&](const auto& val) { newStore[var.name] = val; }, var.defaultValue);
		}
	}

	m_gvTable["_store"] = newStore;
	LDYOM_INFO("global_vars: reset {} variables", proj.has_value() ? proj.value()->globalVars.size() : 0);
}

sol::object GlobalVarsService::getDefault(const std::string& name) {
	if (!m_gvTable.valid())
		return sol::lua_nil;

	auto proj = ProjectsManager::instance().getCurrentProject();
	if (!proj.has_value())
		return sol::lua_nil;

	sol::state_view lua = m_gvTable.lua_state();
	for (const auto& var : proj.value()->globalVars) {
		if (var.name == name) {
			return std::visit([&](const auto& val) -> sol::object { return sol::make_object(lua, val); },
			                  var.defaultValue);
		}
	}
	return sol::lua_nil;
}

void GlobalVarsService::setDefault(const std::string& name, sol::object value) {
	auto proj = ProjectsManager::instance().getCurrentProject();
	if (!proj.has_value())
		return;

	for (auto& var : proj.value()->globalVars) {
		if (var.name != name)
			continue;
		switch (var.type()) {
			case GlobalVarType::Number:
				if (value.is<double>())
					var.defaultValue = value.as<double>();
				break;
			case GlobalVarType::String:
				if (value.is<std::string>())
					var.defaultValue = value.as<std::string>();
				break;
			case GlobalVarType::Boolean:
				if (value.is<bool>())
					var.defaultValue = value.as<bool>();
				break;
		}
		return;
	}
}

void GlobalVarsService::setNumber(const std::string& name, double value) {
	if (!m_gvTable.valid())
		return;
	m_gvTable["_store"].get<sol::table>()[name] = value;
}

void GlobalVarsService::addNumber(const std::string& name, double value) {
	if (!m_gvTable.valid())
		return;
	sol::table st = m_gvTable["_store"];
	st[name] = st.get_or(name, 0.0) + value;
}

void GlobalVarsService::setString(const std::string& name, const std::string& value) {
	if (!m_gvTable.valid())
		return;
	m_gvTable["_store"].get<sol::table>()[name] = value;
}

void GlobalVarsService::setBool(const std::string& name, bool value) {
	if (!m_gvTable.valid())
		return;
	m_gvTable["_store"].get<sol::table>()[name] = value;
}

double GlobalVarsService::getNumber(const std::string& name, double fallback) {
	if (!m_gvTable.valid())
		return fallback;
	return m_gvTable["_store"].get<sol::table>().get_or(name, fallback);
}

std::string GlobalVarsService::getString(const std::string& name, const std::string& fallback) {
	if (!m_gvTable.valid())
		return fallback;
	return m_gvTable["_store"].get<sol::table>().get_or(name, fallback);
}

bool GlobalVarsService::getBool(const std::string& name, bool fallback) {
	if (!m_gvTable.valid())
		return fallback;
	return m_gvTable["_store"].get<sol::table>().get_or(name, fallback);
}
