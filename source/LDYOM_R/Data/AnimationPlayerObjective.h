#pragma once
#include "PlayerObjective.h"

class AnimationPlayerObjective final : public virtual PlayerObjective {
	int pack_ = 0;
	int animation_ = 0;
	bool looped_ = false;
	float blend_ = 0.0f;
	float time_ = 0.f;

public:
	AnimationPlayerObjective() = default;
	explicit AnimationPlayerObjective(void *_new);
	~AnimationPlayerObjective() override = default;

	int getTypeCategory() override {
		return 1;
	}

	int& getPack();
	int& getAnimation();
	bool& isLooped();
	float& getBlend();
	float& getTime();

	void draw(Localization &local, std::vector<std::string> &listOverlay) override;
	ktwait execute(Scene *scene, Result &result, ktcoro_tasklist &tasklist) override;

	void close() override;
};

NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<AnimationPlayerObjective> {
		static void to_json(json &j, const AnimationPlayerObjective &obj) {
			auto &playerObjective = static_cast<const PlayerObjective&>(obj);
			adl_serializer<PlayerObjective>::to_json(j, playerObjective);
			auto &a = const_cast<AnimationPlayerObjective&>(obj);
			j["pack"] = a.getPack();
			j["animation"] = a.getAnimation();
			j["looped"] = a.isLooped();
			j["blend"] = a.getBlend();
			j["time"] = a.getTime();
		}

		static void from_json(const json &j, AnimationPlayerObjective &obj) {
			auto &playerObjective = static_cast<PlayerObjective&>(obj);
			j.get_to(playerObjective);
			j.at("pack").get_to(obj.getPack());
			j.at("animation").get_to(obj.getAnimation());
			j.at("looped").get_to(obj.isLooped());
			j.at("blend").get_to(obj.getBlend());
			j.at("time").get_to(obj.getTime());
		}
	};

NLOHMANN_JSON_NAMESPACE_END
