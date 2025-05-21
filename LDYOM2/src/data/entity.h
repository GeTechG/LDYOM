#pragma once
#include "component.h"
#include <array>
#include <components_manager.h>
#include <lua_define_type.h>
#include <nlohmann/json.hpp>
#include <sol/sol.hpp>
#include <string>
#include <vector>

struct ComponentsContainer {
	std::vector<std::shared_ptr<Component>> data;
};

namespace nlohmann {
void to_json(json& j, const ComponentsContainer& p);
void from_json(const json& j, ComponentsContainer& p);
} // namespace nlohmann

class Entity {
  public:
	std::string name;
	std::array<float, 3> position = {0.0f, 0.0f, 0.0f};
	std::array<float, 3> rotation = {0.0f, 0.0f, 0.0f};
	std::array<float, 3> scale = {1.0f, 1.0f, 1.0f};
	ComponentsContainer components;
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Entity, name, position, rotation, scale, components)
	SOL_LUA_DEFINE_TYPE_INTRUSIVE(Entity, name, position, rotation, scale, components)
};
