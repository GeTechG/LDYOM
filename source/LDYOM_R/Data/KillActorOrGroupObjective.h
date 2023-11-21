#pragma once
#include "ActorObjective.h"

class KillActorOrGroupObjective final : virtual public ActorObjective {
private:
	bool killGroup_ = false;

public:
	KillActorOrGroupObjective() = default;
	explicit KillActorOrGroupObjective(void *_new);
	~KillActorOrGroupObjective() override = default;

	int getTypeCategory() override {
		return 0;
	}

	void draw(Localization &local, std::vector<std::string> &listOverlay) override;
	ktwait execute(Scene *scene, Actor *actor, Result &result, ktcoro_tasklist &tasklist) override;

	bool& isKillGroup();
};

NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<KillActorOrGroupObjective> {
		static void to_json(json &j, const KillActorOrGroupObjective &obj) {
			auto &actorObjective = static_cast<const ActorObjective&>(obj);
			j = actorObjective; // Serialize ActorObjective part
			auto &a = const_cast<KillActorOrGroupObjective&>(obj);
			j["killGroup"] = a.isKillGroup();
		}

		static void from_json(const json &j, KillActorOrGroupObjective &obj) {
			auto &actorObjective = static_cast<ActorObjective&>(obj);
			j.get_to(actorObjective); // Deserialize ActorObjective part)

			j.at("killGroup").get_to(obj.isKillGroup());
		}
	};

NLOHMANN_JSON_NAMESPACE_END
