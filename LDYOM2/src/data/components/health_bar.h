#pragma once
#include "component.h"
#include <components_manager.h>
#include <fa_icons.h>
#include <imgui.h>
#include <localization.h>
#include <lua_define_type.h>
#include <string>

namespace components {

class HealthBar : public Component {
  public:
	static constexpr auto TYPE = "health_bar";
	static constexpr auto CATEGORY = "entities";

	static std::shared_ptr<HealthBar> cast(std::shared_ptr<Component> component) {
		return std::dynamic_pointer_cast<HealthBar>(component);
	}

	static Dependencies getDependencies();

	bool defaultTitle = true;
	std::string title = "";

	std::optional<int> counterId;

	HealthBar();

	[[nodiscard]] nlohmann::json to_json() const override;

	void from_json(const nlohmann::json& j) override;

	void editorRender() override;

	void onStart() override;
	void onUpdate(float deltaTime) override;
	void onReset() override;

	static void sol_lua_register(sol::state_view lua_state);
	static std::shared_ptr<Component> make() { return std::make_shared<HealthBar>(); }
};
} // namespace components