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

class ObjectTriggerMove : public Component {
  private:
	float progress = 0.f;
	std::optional<int> sphereHandle;
	bool dirtySphere = false;

  public:
	static constexpr auto TYPE = "object_trigger_move";
	static constexpr auto CATEGORY = "object";

	static std::shared_ptr<ObjectTriggerMove> cast(std::shared_ptr<Component> component) {
		return std::dynamic_pointer_cast<ObjectTriggerMove>(component);
	}

	static Dependencies getDependencies();

	int radius = 10;
	float time = 1.f;
	std::array<float, 3> endPosition = {0.f, 0.f, 0.f};
	CQuaternion endRotation = {{0.f, 0.f, 0.f}, 0.f};

	ObjectTriggerMove();

	[[nodiscard]] nlohmann::json to_json() const override;

	void from_json(const nlohmann::json& j) override;

	void editorRender() override;

	void onStart() override;
	void onUpdate(float deltaTime) override;
	void onReset() override;

	static void sol_lua_register(sol::state_view lua_state);
	static std::shared_ptr<Component> make() { return std::make_shared<ObjectTriggerMove>(); }
};
} // namespace components