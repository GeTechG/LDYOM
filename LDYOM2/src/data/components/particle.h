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

class Particle : public Component {
  private:
	void updateRotation();
	void updatePosition();

	std::optional<rocket::scoped_connection> onSpawnedConnection;
	std::optional<rocket::scoped_connection> onDespawnedConnection;

  public:
	enum DirtyFlags {
		None = 0,
		Position = 1 << 0,
		Rotation = 1 << 1,
		Type = 1 << 2,
	};
	SOL_LUA_DEFINE_ENUM_NAMED(DirtyFlags, "ParticleComponentDirtyFlags", None, Position, Rotation, Type);

	static constexpr auto TYPE = "particle";
	static constexpr auto CATEGORY = "entities";

	static std::shared_ptr<Particle> cast(std::shared_ptr<Component> component) {
		return std::dynamic_pointer_cast<Particle>(component);
	}

	static Dependencies getDependencies() { return Dependencies{{}, true}; }

	std::string particleType = "";

	int dirty = DirtyFlags::None;
	std::optional<int> handle;

	rocket::thread_safe_signal<void()> onSpawned;
	rocket::thread_safe_signal<void()> onDespawned;

	Particle();

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

	static std::shared_ptr<Component> make() { return std::make_shared<Particle>(); }
};
} // namespace components