#pragma once
#include "PlayerObjective.h"

class RemoveWeaponsObjective final: public virtual PlayerObjective {
private:
	friend class boost::serialization::access;

	template <class Archive>
	void serialize(Archive& ar, const unsigned version) {
		ar & boost::serialization::base_object<PlayerObjective>(*this);
	}

public:
	RemoveWeaponsObjective() = default;
	explicit RemoveWeaponsObjective(void* _new);
	~RemoveWeaponsObjective() override = default;

	int getTypeCategory() override {
		return 4;
	}

	void draw(Localization & local) override;
	ktwait execute(Scene * scene, Result & result, ktcoro_tasklist & tasklist) override;
};
