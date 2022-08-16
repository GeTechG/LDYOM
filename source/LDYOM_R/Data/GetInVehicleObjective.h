#pragma once
#include "VehicleObjective.h"

class GetInVehicleObjective: virtual public VehicleObjective {
private:
	friend class boost::serialization::access;

	template <class Archive>
	void serialize(Archive& ar, const unsigned version) {
		ar& boost::serialization::base_object<VehicleObjective>(*this);
	}

public:
	GetInVehicleObjective() = default;
	explicit GetInVehicleObjective(void* _new);
	~GetInVehicleObjective() override = default;

	int getTypeCategory() override {
		return 0;
	}

	ktwait execute(Scene* scene, Vehicle* vehicle, Result& result, ktcoro_tasklist& tasklist) override;
};
