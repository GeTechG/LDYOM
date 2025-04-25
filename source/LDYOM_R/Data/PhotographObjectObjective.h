#pragma once
#include "ObjectObjective.h"

class PhotographObjectObjective final : virtual public ObjectObjective {
public:
	PhotographObjectObjective() = default;
	explicit PhotographObjectObjective(void *_new);
	~PhotographObjectObjective() override = default;

	int getTypeCategory() override {
		return 1;
	}

	ktwait execute(Scene *scene, Object *object, Result &result, ktcoro_tasklist &tasklist) override;
};


NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<PhotographObjectObjective> {
		static void to_json(json &j, const PhotographObjectObjective &obj) {
			auto &objectObjective = static_cast<const ObjectObjective&>(obj);
			adl_serializer<ObjectObjective>::to_json(j, objectObjective);
			// No additional members in PhotographObjectObjective to serialize
		}

		static void from_json(const json &j, PhotographObjectObjective &obj) {
			auto &objectObjective = static_cast<ObjectObjective&>(obj);
			adl_serializer<ObjectObjective>::from_json(j, objectObjective);
			// No additional members in PhotographObjectObjective to deserialize
		}
	};

NLOHMANN_JSON_NAMESPACE_END
