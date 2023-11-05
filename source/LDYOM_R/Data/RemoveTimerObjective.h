#pragma once
#include "WorldObjective.h"

class RemoveTimerObjective final: public virtual WorldObjective {
	friend class boost::serialization::access;

	template <class Archive>
	void serialize(Archive& ar, const unsigned version) {
		ar & boost::serialization::base_object<WorldObjective>(*this);
	}

public:
	RemoveTimerObjective() = default;
	explicit RemoveTimerObjective(void* _new);
	~RemoveTimerObjective() override = default;

	int getTypeCategory() override {
		return 8;
	}

	void draw(Localization &local, std::vector<std::string> &listOverlay) override;
	ktwait execute(Scene * scene, Result & result, ktcoro_tasklist & tasklist) override;
};
