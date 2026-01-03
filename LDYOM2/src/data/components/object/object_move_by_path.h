#pragma once
#include "component.h"
#include <CQuaternion.h>
#include <components_manager.h>
#include <fa_icons.h>
#include <imgui.h>
#include <localization.h>
#include <lua_define_type.h>
#include <rocket.hpp>
#include <string>

namespace components {

class PathPoint {
  public:
	std::array<float, 3> position = {0.f, 0.f, 0.f};
	CQuaternion rotation = {{0.f, 0.f, 0.f}, 1.f};

	bool operator==(const PathPoint& other) const {
		auto equalImag = rotation.imag.x == other.rotation.imag.x && rotation.imag.y == other.rotation.imag.y &&
		                 rotation.imag.z == other.rotation.imag.z;
		return position == other.position && equalImag && rotation.real == other.rotation.real;
	}

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(PathPoint, position, rotation)
};

class ObjectMoveByPath : public Component {
  private:
	int currentPointIndex = 0;
	float currentProgress = 0.f;
	std::array<float, 3> currentPosition = {0.f, 0.f, 0.f};
	CQuaternion currentRotation = {{0.f, 0.f, 0.f}, 1.f};

	// Runtime variables
	PathPoint initialPoint;        // Initial object spawn position (used as point 0)
	PathPoint startPoint;          // Current segment start point
	PathPoint endPoint;            // Current segment end point
	bool isMoving = false;         // Flag to track if movement is active
	bool initialPointPassed = false; // Flag to track if initial point was already used

  public:
	static constexpr auto TYPE = "object_move_by_path";
	static constexpr auto CATEGORY = "object";

	static std::shared_ptr<ObjectMoveByPath> cast(std::shared_ptr<Component> component) {
		return std::dynamic_pointer_cast<ObjectMoveByPath>(component);
	}

	static Dependencies getDependencies();

	std::vector<PathPoint> points;
	int speedType = 1; // 0: Slow, 1: Normal, 2: Fast
	bool loop = false;
	bool autoStart = true;

	ObjectMoveByPath();

	void start();
	void stop();
	void reset();

	[[nodiscard]] nlohmann::json to_json() const override;

	void from_json(const nlohmann::json& j) override;

	void editorRender() override;

	void onStart() override;
	void onUpdate(float deltaTime) override;
	void onReset() override;

	static void sol_lua_register(sol::state_view lua_state);
	static std::shared_ptr<Component> make() { return std::make_shared<ObjectMoveByPath>(); }
};
} // namespace components
