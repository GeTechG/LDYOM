#pragma once
#include "PlayerObjective.h"

class LevelWantedPlayerObjective final : public virtual PlayerObjective {
private:
	int levelWanted_ = 0;

public:
	LevelWantedPlayerObjective() = default;
	explicit LevelWantedPlayerObjective(void *_new);
	~LevelWantedPlayerObjective() override = default;

	int getTypeCategory() override {
		return 3;
	}

	int& getLevelWanted();

	void draw(Localization &local, std::vector<std::string> &listOverlay) override;
	ktwait execute(Scene *scene, Result &result, ktcoro_tasklist &tasklist) override;
};


NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<LevelWantedPlayerObjective> {
		static void to_json(json &j, const LevelWantedPlayerObjective &obj) {
			auto &playerObjective = static_cast<const PlayerObjective&>(obj);
			adl_serializer<PlayerObjective>::to_json(j, playerObjective);
			auto &a = const_cast<LevelWantedPlayerObjective&>(obj);
			j["levelWanted"] = a.getLevelWanted(); // Assuming getLevelWanted() returns the levelWanted_ member.
		}

		static void from_json(const json &j, LevelWantedPlayerObjective &obj) {
			auto &playerObjective = static_cast<PlayerObjective&>(obj);
			j.get_to(playerObjective);
			j.at("levelWanted").get_to(obj.getLevelWanted());
			// Assuming getLevelWanted() exists to set the levelWanted_ member.
		}
	};

NLOHMANN_JSON_NAMESPACE_END
