#pragma once
#include <lua_define_type.h>
#include <nlohmann/json.hpp>
#include <string>

class Entity;

struct Dependencies {
	std::vector<std::string> components;
	bool oneOf = false;
	SOL_LUA_DEFINE_TYPE_INTRUSIVE(Dependencies, components, oneOf)
};

class Component {
  protected:
	std::string tr(std::string_view key);

  public:
	virtual ~Component() = default;
	bool isInitialized = false;
	std::string type;
	Entity* entity = nullptr;

	Component(const std::string_view type)
		: type(std::string(type)) {}

	[[nodiscard]] std::string getType() const { return type; }

	[[nodiscard]] virtual nlohmann::json to_json() const { return nlohmann::json{{"type", type}}; }

	std::string getName() { return tr("name"); }

	virtual void from_json(const nlohmann::json& j) { j.at("type").get_to(type); }

	virtual void editorRender() = 0;
	virtual void onStart() { isInitialized = true; }
	virtual void onUpdate(float deltaTime) {}
	virtual void onReset() { isInitialized = false; }

	static void sol_lua_register(sol::state_view lua_state);
};