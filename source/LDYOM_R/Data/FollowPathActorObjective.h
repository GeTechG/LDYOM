#pragma once
#include "ActorObjective.h"

class FollowPathActorObjective final : public virtual ActorObjective {
private:
	int moveType_ = 0;
	int pathType_ = 0;
	float executeTime_ = 1.f;
	std::vector<std::array<float, 3>> path_;
	bool waitEnd = false;

public:
	FollowPathActorObjective() = default;
	explicit FollowPathActorObjective(void *_new);
	~FollowPathActorObjective() override = default;

	int getTypeCategory() override {
		return 2;
	}

	void draw(Localization &local, std::vector<std::string> &listOverlay) override;
	ktwait execute(Scene *scene, Actor *actor, Result &result, ktcoro_tasklist &tasklist) override;

	int& getMoveType();
	int& getPathType();
	float& getExecuteTime();
	std::vector<std::array<float, 3>>& getPath();
	bool& isWaitEnd();
};

NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<FollowPathActorObjective> {
		static void to_json(json &j, const FollowPathActorObjective &obj) {
			auto &actorObjective = static_cast<const ActorObjective&>(obj);
			adl_serializer<ActorObjective>::to_json(j, actorObjective);
			auto &a = const_cast<FollowPathActorObjective&>(obj);
			j["moveType"] = a.getMoveType();
			j["pathType"] = a.getPathType();
			j["executeTime"] = a.getExecuteTime();
			j["path"] = a.getPath();
			j["waitEnd"] = a.isWaitEnd();
		}

		static void from_json(const json &j, FollowPathActorObjective &obj) {
			auto &actorObjective = static_cast<ActorObjective&>(obj);
			j.get_to(actorObjective);

			j.at("moveType").get_to(obj.getMoveType());
			j.at("pathType").get_to(obj.getPathType());
			j.at("executeTime").get_to(obj.getExecuteTime());
			j.at("path").get_to(obj.getPath());
			j.at("waitEnd").get_to(obj.isWaitEnd());
		}
	};

NLOHMANN_JSON_NAMESPACE_END
