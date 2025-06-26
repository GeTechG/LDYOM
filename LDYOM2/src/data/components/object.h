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

class Object : public Component {
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
		Model = 1 << 2,
	};
	SOL_LUA_DEFINE_ENUM_NAMED(DirtyFlags, "ObjectComponentDirtyFlags", None, Position, Rotation, Model);

	static constexpr auto TYPE = "object";
	static constexpr auto CATEGORY = "entities";

	static std::shared_ptr<Object> cast(std::shared_ptr<Component> component) {
		return std::dynamic_pointer_cast<Object>(component);
	}

	static Dependencies getDependencies() { return Dependencies{{}, true}; }

	int model = 0;

	int dirty = DirtyFlags::None;
	std::shared_ptr<CObject> handle;

	rocket::thread_safe_signal<void()> onSpawned;
	rocket::thread_safe_signal<void()> onDespawned;

	Object();

	[[nodiscard]] nlohmann::json to_json() const override;

	void from_json(const nlohmann::json& j) override;

	void editorRender() override;

	void onStart() override;
	void onUpdate(float deltaTime) override;
	void onReset() override;

	void spawn();
	void despawn();
	int getObjectRef() const { return this->handle ? CPools::GetObjectRef(this->handle.get()) : -1; }

	static bool isSpecialComponent() { return true; }
	static void sol_lua_register(sol::state_view lua_state);

	static std::shared_ptr<Component> make() { return std::make_shared<Object>(); }
};
} // namespace components