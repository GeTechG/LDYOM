#pragma once
#include "WorldObjective.h"

class ClockTimeObjective final : virtual public WorldObjective {
private:
	friend class boost::serialization::access;

	template <class Archive>
	void serialize(Archive& ar, const unsigned version) {
		ar & boost::serialization::base_object<WorldObjective>(*this);
		ar & boost::serialization::make_array(time_.data(), time_.size());
	}

	std::array<signed char, 2> time_ = {8, 0};
public:
	ClockTimeObjective() = default;
	explicit ClockTimeObjective(void* _new);
	~ClockTimeObjective() override = default;

	int getTypeCategory() override {
		return 5;
	}

	void draw(Localization &local, std::vector<std::string> &listOverlay) override;
	ktwait execute(Scene * scene, Result & result, ktcoro_tasklist & tasklist) override;
};
