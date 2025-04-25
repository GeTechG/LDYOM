#include <sol.hpp>
#include "../Data/Weapon.h"

void weaponWrapper(sol::state &state) {
	state.new_usertype<Weapon>("Weapon",
	                           sol::no_constructor,
	                           "weapon", &Weapon::weapon,
	                           "ammo", &Weapon::ammo
	);
}
