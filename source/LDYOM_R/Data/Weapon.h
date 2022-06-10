#pragma once

struct Weapon {
	int weapon;
	int ammo;
};

namespace boost {
	namespace serialization {

		template<class Archive>
		void serialize(Archive& ar, Weapon& w, const unsigned int version)
		{
			ar & w.weapon;
			ar & w.ammo;
		}

	}
}