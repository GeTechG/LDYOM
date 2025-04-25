#pragma once
#include <boost/serialization/base_object.hpp>

#include "BaseObjective.h"

class WorldObjective : virtual public BaseObjective {
protected:
	WorldObjective() = default;
	WorldObjective(void *_new);

public:
	int getCategory() override {
		return 0;
	}
};

NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<WorldObjective> {
		static void to_json(json &j, const WorldObjective &obj) {
			auto &baseObjective = static_cast<const BaseObjective&>(obj);
			adl_serializer<BaseObjective>::to_json(j, baseObjective);
		}

		static void from_json(const json &j, WorldObjective &obj) {
			auto &baseObjective = static_cast<BaseObjective&>(obj);
			j.get_to(baseObjective);
		}
	};

NLOHMANN_JSON_NAMESPACE_END
