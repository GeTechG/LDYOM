#pragma once
#include "component.h"
#include <components_manager.h>
#include <fa_icons.h>
#include <imgui.h>
#include <localization.h>
#include <lua_define_type.h>
#include <rocket.hpp>
#include <string>

namespace components {

class ActorBehaviour : public Component {
  private:
	std::optional<rocket::scoped_connection> m_pedSpawnedConnection;

  public:
	static constexpr auto TYPE = "actor_behaviour";
	static constexpr auto CATEGORY = "actor";

	static std::shared_ptr<ActorBehaviour> cast(std::shared_ptr<Component> component) {
		return std::dynamic_pointer_cast<ActorBehaviour>(component);
	}

	static Dependencies getDependencies();

	bool holdPosition = false;
	bool directAttack = false;
	bool followPlayer = false;

	ActorBehaviour();

	void applyBehaviour();

	[[nodiscard]] nlohmann::json to_json() const override;

	void from_json(const nlohmann::json& j) override;

	void editorRender() override;

	void onStart() override;
	void onUpdate(float deltaTime) override;
	void onReset() override;

	static void sol_lua_register(sol::state_view lua_state);
	static std::shared_ptr<Component> make() { return std::make_shared<ActorBehaviour>(); }
};
} // namespace components