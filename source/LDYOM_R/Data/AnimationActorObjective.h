#pragma once
#include "ActorObjective.h"

class AnimationActorObjective final : public virtual ActorObjective {
private:
	int pack = 0;
	int animation = 0;
	bool looped = false;
	float blend = 0.0f;
	float time = 0.f;
	bool endWait = false;

public:
	AnimationActorObjective() = default;
	explicit AnimationActorObjective(void *_new);
	~AnimationActorObjective() override = default;

	int getTypeCategory() override {
		return 3;
	}

	int& getPack();
	int& getAnimation();
	bool& isLooped();
	float& getBlend();
	float& getTime();
	bool& isEndWait();

	void draw(Localization &local, std::vector<std::string> &listOverlay) override;
	ktwait execute(Scene *scene, Actor *actor, Result &result, ktcoro_tasklist &tasklist) override;

	void close() override;
};

NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<AnimationActorObjective> {
		static void to_json(json &j, const AnimationActorObjective &obj) {
			auto &actorObjective = static_cast<const ActorObjective&>(obj);
			adl_serializer<ActorObjective>::to_json(j, actorObjective);
			auto &a = const_cast<AnimationActorObjective&>(obj);
			j["pack"] = a.getPack();
			j["animation"] = a.getAnimation();
			j["looped"] = a.isLooped();
			j["blend"] = a.getBlend();
			j["time"] = a.getTime();
			j["endWait"] = a.isEndWait();
		}

		static void from_json(const json &j, AnimationActorObjective &obj) {
			auto &actorObjective = static_cast<ActorObjective&>(obj);
			adl_serializer<ActorObjective>::from_json(j, actorObjective);

			j.at("pack").get_to(obj.getPack());
			j.at("animation").get_to(obj.getAnimation());
			j.at("looped").get_to(obj.isLooped());
			j.at("blend").get_to(obj.getBlend());
			j.at("time").get_to(obj.getTime());
			j.at("endWait").get_to(obj.isEndWait());
		}
	};

NLOHMANN_JSON_NAMESPACE_END
