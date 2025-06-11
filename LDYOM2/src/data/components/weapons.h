#pragma once
#include "component.h"
#include <components_manager.h>
#include <fa_icons.h>
#include <imgui.h>
#include <localization.h>
#include <lua_define_type.h>
#include <optional>
#include <rocket.hpp>
#include <string>

namespace components {

class Weapons : public Component {
	std::optional<rocket::scoped_connection> m_pedSpawnedConnection;

  public:
	enum DirtyFlags {
		None = 0,
		GiveWeapons = 1 << 0,
		UpdateDefaultWeapon = 1 << 1,
	};

	SOL_LUA_DEFINE_ENUM_NAMED(DirtyFlags, "WeaponsComponentDirtyFlags", None, GiveWeapons, UpdateDefaultWeapon);

	static constexpr auto TYPE = "weapons";
	static constexpr auto CATEGORY = "actor";

	static std::shared_ptr<Weapons> cast(std::shared_ptr<Component> component) {
		return std::dynamic_pointer_cast<Weapons>(component);
	}

	static Dependencies getDependencies();

	struct Weapon {
		int weapon;
		int ammo;
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(Weapon, weapon, ammo)
	};

	int dirty = None;
	std::vector<Weapon> weapons;
	int defaultWeapon = 0;
	int accuracy = 50;
	bool isRandom = false;

	Weapons();

	void giveWeapons();
	void updateDefaultWeapon();

	[[nodiscard]] nlohmann::json to_json() const override;

	void from_json(const nlohmann::json& j) override;

	void editorRender() override;

	void onStart() override;
	void onUpdate(float deltaTime) override;
	void onReset() override;

	static void sol_lua_register(sol::state_view lua_state);
	static std::shared_ptr<Component> make() { return std::make_shared<Weapons>(); }
};
} // namespace components