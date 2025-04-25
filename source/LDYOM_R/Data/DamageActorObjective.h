#pragma once
#include "ActorObjective.h"

class DamageActorObjective final : virtual public ActorObjective {
public:
	DamageActorObjective() = default;
	explicit DamageActorObjective(void *_new);
	~DamageActorObjective() override = default;

	int getTypeCategory() override {
		return 1;
	}

	ktwait execute(Scene *scene, Actor *actor, Result &result, ktcoro_tasklist &tasklist) override;
};

NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<DamageActorObjective> {
		static void to_json(json &j, const DamageActorObjective &obj) {
			auto &actorObjective = static_cast<const ActorObjective&>(obj);
			adl_serializer<ActorObjective>::to_json(j, actorObjective);
		}

		static void from_json(const json &j, DamageActorObjective &obj) {
			auto &actorObjective = static_cast<ActorObjective&>(obj);
			adl_serializer<ActorObjective>::from_json(j, actorObjective);
		}
	};

NLOHMANN_JSON_NAMESPACE_END
