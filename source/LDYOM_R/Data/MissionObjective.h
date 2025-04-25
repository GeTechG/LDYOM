#pragma once
#include "BaseObjective.h"

class MissionObjective : virtual public BaseObjective {
public:
	int getCategory() override {
		return 6;
	}
};

NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<MissionObjective> {
		static void to_json(json &j, const MissionObjective &obj) {
			auto &baseObjective = static_cast<const BaseObjective&>(obj);
			adl_serializer<BaseObjective>::to_json(j, baseObjective);
		}

		static void from_json(const json &j, MissionObjective &obj) {
			auto &baseObjective = static_cast<BaseObjective&>(obj);
			adl_serializer<BaseObjective>::from_json(j, baseObjective);
		}
	};

NLOHMANN_JSON_NAMESPACE_END
