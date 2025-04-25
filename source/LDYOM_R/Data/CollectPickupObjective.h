#pragma once
#include "PickupObjective.h"

class CollectPickupObjective final : virtual public PickupObjective {
public:
	CollectPickupObjective() = default;
	explicit CollectPickupObjective(void *_new);
	~CollectPickupObjective() override = default;

	int getTypeCategory() override {
		return 0;
	}

	ktwait execute(Scene *scene, Pickup *pickup, Result &result, ktcoro_tasklist &tasklist) override;
};

NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<CollectPickupObjective> {
		static void to_json(json &j, const CollectPickupObjective &obj) {
			auto &pickupObjective = static_cast<const PickupObjective&>(obj);
			adl_serializer<PickupObjective>::to_json(j, pickupObjective);
			// No additional members in CollectPickupObjective to serialize
		}

		static void from_json(const json &j, CollectPickupObjective &obj) {
			auto &pickupObjective = static_cast<PickupObjective&>(obj);
			adl_serializer<PickupObjective>::from_json(j, pickupObjective);
			// No additional members in CollectPickupObjective to deserialize
		}
	};

NLOHMANN_JSON_NAMESPACE_END
