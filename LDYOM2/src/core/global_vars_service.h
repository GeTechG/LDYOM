#pragma once
#include <string>
#include <sol/sol.hpp>

class GlobalVarsService {
  public:
	static GlobalVarsService& instance();

	// Called once from LuaManager initialization
	void initialize(sol::state_view lua);
	// Reload defaults from current project into _store
	void reset();

	// Operations used by objectives
	void setNumber(const std::string& name, double value);
	void addNumber(const std::string& name, double value);
	void setString(const std::string& name, const std::string& value);
	void setBool(const std::string& name, bool value);

	// Getters for runtime values
	double getNumber(const std::string& name, double fallback = 0.0);
	std::string getString(const std::string& name, const std::string& fallback = {});
	bool getBool(const std::string& name, bool fallback = false);

	// Default value editing (modifies project definition, not runtime store)
	sol::object getDefault(const std::string& name);
	void setDefault(const std::string& name, sol::object value);

  private:
	GlobalVarsService() = default;

	sol::table m_gvTable; // rooted reference to global_vars table
};
