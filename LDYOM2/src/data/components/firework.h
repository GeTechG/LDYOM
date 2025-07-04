#pragma once
#include "component.h"
#include <CObject.h>
#include <CPools.h>
#include <components_manager.h>
#include <fa_icons.h>
#include <imgui.h>
#include <localization.h>
#include <lua_define_type.h>
#include <memory>
#include <models_manager.h>
#include <optional>
#include <rocket.hpp>
#include <string>

namespace components {

class Firework : public Component {
  private:
	void updateRotation();
	void updatePosition();

	std::optional<rocket::scoped_connection> onSpawnedConnection;
	std::optional<rocket::scoped_connection> onDespawnedConnection;

	std::shared_ptr<CObject> m_editorExplosionObject;

	void runExplosion();

  public:
	static constexpr auto TYPE = "firework";
	static constexpr auto CATEGORY = "entities";

	static std::shared_ptr<Firework> cast(std::shared_ptr<Component> component) {
		return std::dynamic_pointer_cast<Firework>(component);
	}

	static Dependencies getDependencies() { return Dependencies{{}, true}; }

	int typeFirework = 0;
	int typeExplosion = 0;
	int sizeFire = 0;
	int propagationFire = 0;

	bool dirty = false;
	std::optional<int> fire;

	rocket::thread_safe_signal<void()> onSpawned;
	rocket::thread_safe_signal<void()> onDespawned;

	Firework();

	[[nodiscard]] nlohmann::json to_json() const override;

	void from_json(const nlohmann::json& j) override;

	void editorRender() override;

	void onStart() override;
	void onUpdate(float deltaTime) override;
	void onReset() override;

	void spawn();
	void despawn();

	static bool isSpecialComponent() { return true; }
	static void sol_lua_register(sol::state_view lua_state);

	static std::shared_ptr<Component> make() { return std::make_shared<Firework>(); }
};
} // namespace components