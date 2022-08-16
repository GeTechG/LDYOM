#pragma once
#include "WorldObjective.h"

class WeatherObjective final : virtual public WorldObjective {
private:
	friend class boost::serialization::access;

	template <class Archive>
	void serialize(Archive& ar, const unsigned version) {
		ar & boost::serialization::base_object<WorldObjective>(*this);
		ar & weather_;
	}

	int weather_ = 0;
public:
	WeatherObjective() = default;
	explicit WeatherObjective(void* _new);
	~WeatherObjective() override = default;

	int getTypeCategory() override {
		return 4;
	}

	void draw(Localization & local) override;
	ktwait execute(Scene * scene, Result & result, ktcoro_tasklist & tasklist) override;
};
