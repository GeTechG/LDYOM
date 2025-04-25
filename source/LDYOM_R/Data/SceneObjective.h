#pragma once
#include "BaseObjective.h"

class SceneObjective : virtual public BaseObjective {
public:
	int getCategory() override {
		return 7;
	}
};

NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<SceneObjective> {
		static void to_json(json &j, const SceneObjective &obj) {
			auto &baseObjective = static_cast<const BaseObjective&>(obj);
			adl_serializer<BaseObjective>::to_json(j, baseObjective);
			// Add additional serialization for SceneObjective if needed
		}

		static void from_json(const json &j, SceneObjective &obj) {
			auto &baseObjective = static_cast<BaseObjective&>(obj);
			j.get_to(baseObjective);
			// Add additional deserialization for SceneObjective if needed
		}
	};

NLOHMANN_JSON_NAMESPACE_END
