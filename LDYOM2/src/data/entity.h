#pragma once
#include <CQuaternion.h>
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
	std::function<std::array<float, 3>()> getPositionCallback;
	std::function<CQuaternion()> getRotationCallback;
	std::function<std::array<float, 3>()> getScaleCallback;

	std::function<void(std::array<float, 3>)> setPositionCallback;
	std::function<void(CQuaternion)> setRotationCallback;
	std::function<void(std::array<float, 3>)> setScaleCallback;

  public:
	std::string name;
	uuids::uuid id = uuids::uuid_system_generator{}();
	std::array<float, 3> position = {0.0f, 0.0f, 0.0f};
	CQuaternion rotation;
	std::array<float, 3> scale = {1.0f, 1.0f, 1.0f};
	int areaId = 0;
	void addComponent(std::shared_ptr<Component> component);
	const std::vector<std::shared_ptr<Component>>& getComponents() const { return components; }
	std::shared_ptr<Component> getComponent(const std::string_view type);
	bool hasComponent(const std::string_view type) { return getComponent(type) != nullptr; }
	void setGetTransformCallbacks(std::function<std::array<float, 3>()> positionCallback,
	                              std::function<CQuaternion()> rotationCallback,
	                              std::function<std::array<float, 3>()> scaleCallback);
	void setSetTransformCallbacks(std::function<void(std::array<float, 3>)> positionCallback,
	                              std::function<void(CQuaternion)> rotationCallback,
	                              std::function<void(std::array<float, 3>)> scaleCallback);
	void clearGetTransformCallbacks() {
		getPositionCallback = nullptr;
		getRotationCallback = nullptr;
		getScaleCallback = nullptr;
	}
	void clearSetTransformCallbacks() {
		setPositionCallback = nullptr;
		setRotationCallback = nullptr;
		setScaleCallback = nullptr;
	}
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
