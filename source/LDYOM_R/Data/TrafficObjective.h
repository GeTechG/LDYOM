#pragma once
#include "WorldObjective.h"

class TrafficObjective final : virtual public WorldObjective {
private:
	friend class boost::serialization::access;

	template <class Archive>
	void serialize(Archive& ar, const unsigned version) {
		ar & boost::serialization::base_object<WorldObjective>(*this);
		ar & trafficPed_;
		ar & trafficCar_;
	}

	float trafficPed_ = 1.f;
	float trafficCar_ = 1.f;
public:
	TrafficObjective() = default;
	explicit TrafficObjective(void* _new);
	~TrafficObjective() override = default;

	int getTypeCategory() override {
		return 6;
	}

	void draw(Localization &local, std::vector<std::string> &listOverlay) override;
	ktwait execute(Scene * scene, Result & result, ktcoro_tasklist & tasklist) override;
};
