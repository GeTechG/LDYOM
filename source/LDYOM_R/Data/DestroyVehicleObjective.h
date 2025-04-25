#pragma once
#include "VehicleObjective.h"

class DestroyVehicleObjective : virtual public VehicleObjective {
private:

public:
	DestroyVehicleObjective() = default;
	explicit DestroyVehicleObjective(void *_new);
	~DestroyVehicleObjective() override = default;

	int getTypeCategory() override {
		return 1;
	}

	ktwait execute(Scene *scene, Vehicle *vehicle, Result &result, ktcoro_tasklist &tasklist) override;
};

NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<DestroyVehicleObjective> {
		static void to_json(json &j, const DestroyVehicleObjective &obj) {
			auto &vehicleObjective = static_cast<const VehicleObjective&>(obj);
			adl_serializer<VehicleObjective>::to_json(j, vehicleObjective);
		}

		static void from_json(const json &j, DestroyVehicleObjective &obj) {
			auto &vehicleObjective = static_cast<VehicleObjective&>(obj);
			j.get_to(vehicleObjective);
		}
	};

NLOHMANN_JSON_NAMESPACE_END
