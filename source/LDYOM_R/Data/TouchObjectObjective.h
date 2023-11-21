#pragma once
#include "ObjectObjective.h"

class TouchObjectObjective final : virtual public ObjectObjective {
public:
	TouchObjectObjective() = default;
	explicit TouchObjectObjective(void *_new);
	~TouchObjectObjective() override = default;

	int getTypeCategory() override {
		return 2;
	}

	ktwait execute(Scene *scene, Object *object, Result &result, ktcoro_tasklist &tasklist) override;
};

NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<TouchObjectObjective> {
		static void to_json(json &j, const TouchObjectObjective &obj) {
			auto &objectObjective = static_cast<const ObjectObjective&>(obj);
			adl_serializer<ObjectObjective>::to_json(j, objectObjective);
			// No additional members in TouchObjectObjective to serialize
		}

		static void from_json(const json &j, TouchObjectObjective &obj) {
			auto &objectObjective = static_cast<ObjectObjective&>(obj);
			adl_serializer<ObjectObjective>::from_json(j, objectObjective);
			// No additional members in TouchObjectObjective to deserialize
		}
	};

NLOHMANN_JSON_NAMESPACE_END
