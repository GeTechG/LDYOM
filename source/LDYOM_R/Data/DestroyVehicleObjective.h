#pragma once
#include "VehicleObjective.h"

class DestroyVehicleObjective: virtual public VehicleObjective {
private:
	friend class boost::serialization::access;

	template <class Archive>
	void serialize(Archive& ar, const unsigned version) {
		ar& boost::serialization::base_object<VehicleObjective>(*this);
	}

public:
	DestroyVehicleObjective() = default;
	explicit DestroyVehicleObjective(void* _new);
	~DestroyVehicleObjective() override = default;

	int getTypeCategory() override {
		return 1;
	}

	ktwait execute(Scene* scene, Vehicle* vehicle, Result& result, ktcoro_tasklist& tasklist) override;
};
