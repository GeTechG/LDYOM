#pragma once
#include "BaseObjective.h"

class PlayerObjective : virtual public BaseObjective {
protected:
	PlayerObjective() = default;

public:
	int getCategory() override {
		return 5;
	}
};

NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<PlayerObjective> {
		static void to_json(json &j, const PlayerObjective &obj) {
			auto &baseObjective = static_cast<const BaseObjective&>(obj);
			adl_serializer<BaseObjective>::to_json(j, baseObjective);
			// No additional members in PlayerObjective to serialize
		}

		static void from_json(const json &j, PlayerObjective &obj) {
			auto &baseObjective = static_cast<BaseObjective&>(obj);
			adl_serializer<BaseObjective>::from_json(j, baseObjective);
			// No additional members in PlayerObjective to deserialize
		}
	};

NLOHMANN_JSON_NAMESPACE_END
