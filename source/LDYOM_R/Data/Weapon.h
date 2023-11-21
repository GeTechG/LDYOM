#pragma once
#include <nlohmann/json.hpp>

struct Weapon {
	int weapon;
	int ammo;
};

NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<Weapon> {
		static void to_json(json &j, const Weapon &w) {
			j = json{
				{"weapon", w.weapon},
				{"ammo", w.ammo}
			};
		}

		static void from_json(const json &j, Weapon &w) {
			j.at("weapon").get_to(w.weapon);
			j.at("ammo").get_to(w.ammo);
		}
	};

NLOHMANN_JSON_NAMESPACE_END
