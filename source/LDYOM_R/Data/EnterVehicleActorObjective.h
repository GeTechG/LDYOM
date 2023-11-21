#pragma once
#include "ActorObjective.h"

class EnterVehicleActorObjective final : public virtual ActorObjective {
private:
	boost::uuids::uuid vehicleUuid_{};
	int place_ = 0;
	bool teleport_ = false;
	bool waitExecute_ = false;

public:
	EnterVehicleActorObjective() = default;
	explicit EnterVehicleActorObjective(void *_new);
	~EnterVehicleActorObjective() override = default;

	int getTypeCategory() override {
		return 4;
	}

	boost::uuids::uuid& getVehicleUuid();
	int& getPlace();
	bool& isTeleport();
	bool& isWaitExecute();

	void draw(Localization &local, std::vector<std::string> &listOverlay) override;
	ktwait execute(Scene *scene, Actor *actor, Result &result, ktcoro_tasklist &tasklist) override;
};

NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<EnterVehicleActorObjective> {
		static void to_json(json &j, const EnterVehicleActorObjective &obj) {
			auto &actorObjective = static_cast<const ActorObjective&>(obj);
			adl_serializer<ActorObjective>::to_json(j, actorObjective);
			auto &a = const_cast<EnterVehicleActorObjective&>(obj);
			j["vehicleUuid"] = a.getVehicleUuid();
			j["place"] = a.getPlace();
			j["teleport"] = a.isTeleport();
			j["waitExecute"] = a.isWaitExecute();
		}

		static void from_json(const json &j, EnterVehicleActorObjective &obj) {
			auto &actorObjective = static_cast<ActorObjective&>(obj);
			adl_serializer<ActorObjective>::from_json(j, actorObjective);

			j.at("vehicleUuid").get_to(obj.getVehicleUuid());
			j.at("place").get_to(obj.getPlace());
			j.at("teleport").get_to(obj.isTeleport());
			j.at("waitExecute").get_to(obj.isWaitExecute());
		}
	};

NLOHMANN_JSON_NAMESPACE_END
