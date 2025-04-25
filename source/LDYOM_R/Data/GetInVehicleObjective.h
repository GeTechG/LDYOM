#pragma once
#include "VehicleObjective.h"

class GetInVehicleObjective : virtual public VehicleObjective {
private:

public:
	GetInVehicleObjective() = default;
	explicit GetInVehicleObjective(void *_new);
	~GetInVehicleObjective() override = default;

	int getTypeCategory() override {
		return 0;
	}

	ktwait execute(Scene *scene, Vehicle *vehicle, Result &result, ktcoro_tasklist &tasklist) override;
};


NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<GetInVehicleObjective> {
		static void to_json(json &j, const GetInVehicleObjective &obj) {
			auto &vehicleObjective = static_cast<const VehicleObjective&>(obj);
			adl_serializer<VehicleObjective>::to_json(j, vehicleObjective);
		}

		static void from_json(const json &j, GetInVehicleObjective &obj) {
			auto &vehicleObjective = static_cast<VehicleObjective&>(obj);
			j.get_to(vehicleObjective);
		}
	};

NLOHMANN_JSON_NAMESPACE_END
