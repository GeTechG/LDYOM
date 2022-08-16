#pragma once
#include "PlayerObjective.h"

class AddMoneyPlayerObjective final: public virtual PlayerObjective {
private:
	friend class boost::serialization::access;

	template <class Archive>
	void serialize(Archive& ar, const unsigned version) {
		ar & boost::serialization::base_object<PlayerObjective>(*this);
		ar & money_;
	}

	int money_ = 0;
public:
	AddMoneyPlayerObjective() = default;
	explicit AddMoneyPlayerObjective(void* _new);
	~AddMoneyPlayerObjective() override = default;

	int getTypeCategory() override {
		return 7;
	}

	void draw(Localization & local) override;
	ktwait execute(Scene * scene, Result & result, ktcoro_tasklist & tasklist) override;
};
