#pragma once
#include <array>
#include <lua_define_type.h>
#include <nlohmann/json.hpp>
#include <sol/sol.hpp>
#include <string>
#include <uuid_wrap.h>
#include <vector>

class Component;

class Entity {
  private:
	std::vector<std::shared_ptr<Component>> components;
	std::array<std::function<float*()>, 3> getTransformCallbacks;
	std::array<std::function<void(float*)>, 3> setTransformCallbacks;

  public:
	std::string name;
	uuids::uuid id = uuids::uuid_system_generator{}();
	std::array<float, 3> position = {0.0f, 0.0f, 0.0f};
	std::array<float, 3> rotation = {0.0f, 0.0f, 0.0f};
	std::array<float, 3> scale = {1.0f, 1.0f, 1.0f};
	int areaId = 0;
	void addComponent(std::shared_ptr<Component> component);
	const std::vector<std::shared_ptr<Component>>& getComponents() const { return components; }
	std::shared_ptr<Component> getComponent(const std::string_view type);
	bool hasComponent(const std::string_view type) { return getComponent(type) != nullptr; }
	void setGetTransformCallbacks(std::function<float*()> positionCallback, std::function<float*()> rotationCallback,
	                              std::function<float*()> scaleCallback);
	void setSetTransformCallbacks(std::function<void(float*)> positionCallback,
	                              std::function<void(float*)> rotationCallback,
	                              std::function<void(float*)> scaleCallback);
	void clearGetTransformCallbacks() { getTransformCallbacks.fill(nullptr); }
	void clearSetTransformCallbacks() { setTransformCallbacks.fill(nullptr); }
	void updateSetTransformCallbacks();

	// Component management operations
	void removeComponent(size_t index);
	void duplicateComponent(size_t index);
	void moveComponentUp(size_t index);
	void moveComponentDown(size_t index);

	void onUpdate(float deltaTime);
	void reset();

	friend void to_json(nlohmann::json& j, const Entity& p);
	friend void from_json(const nlohmann::json& j, Entity& p);
	static void sol_lua_register(sol::state_view lua_state);
};
