#pragma once
#include "PlayerObjective.h"
#include "Weapon.h"

class AddWeaponsPlayerObjective final: public virtual PlayerObjective {
private:
	friend class boost::serialization::access;

	template <class Archive>
	void serialize(Archive& ar, const unsigned version) {
		ar & boost::serialization::base_object<PlayerObjective>(*this);
		ar & weapons_;
		ar & defaultWeapon_;
	}

	std::vector<Weapon> weapons_;
	int defaultWeapon_{};
public:
	AddWeaponsPlayerObjective() = default;
	explicit AddWeaponsPlayerObjective(void* _new);
	~AddWeaponsPlayerObjective() override = default;

	int getTypeCategory() override {
		return 5;
	}

	void draw(Localization &local, std::vector<std::string> &listOverlay) override;
	ktwait execute(Scene * scene, Result & result, ktcoro_tasklist & tasklist) override;
};
