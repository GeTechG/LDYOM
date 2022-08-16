#pragma once
#include "PickupObjective.h"

class CollectPickupObjective final : virtual public PickupObjective {
private:
	friend class boost::serialization::access;

	template <class Archive>
	void serialize(Archive& ar, const unsigned version) {
		ar & boost::serialization::base_object<PickupObjective>(*this);
	}

public:
	CollectPickupObjective() = default;
	explicit CollectPickupObjective(void* _new);
	~CollectPickupObjective() override = default;

	int getTypeCategory() override {
		return 0;
	}

	ktwait execute(Scene* scene, Pickup* pickup, Result& result, ktcoro_tasklist& tasklist) override;
};
