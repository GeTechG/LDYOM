#pragma once
#include "PlayerObjective.h"

class TeleportToVehiclePlayerObjective final : public virtual PlayerObjective {
private:
	boost::uuids::uuid vehicleUuid_;
	int place_ = 0;

public:
	TeleportToVehiclePlayerObjective() = default;
	explicit TeleportToVehiclePlayerObjective(void *_new);
	~TeleportToVehiclePlayerObjective() override = default;

	int getTypeCategory() override {
		return 2;
	}

	boost::uuids::uuid& getVehicleUuid();
	int& getPlace();

	void draw(Localization &local, std::vector<std::string> &listOverlay) override;
	ktwait execute(Scene *scene, Result &result, ktcoro_tasklist &tasklist) override;
};

NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<TeleportToVehiclePlayerObjective> {
		static void to_json(json &j, const TeleportToVehiclePlayerObjective &obj) {
			auto &playerObjective = static_cast<const PlayerObjective&>(obj);
			adl_serializer<PlayerObjective>::to_json(j, playerObjective);
			auto &a = const_cast<TeleportToVehiclePlayerObjective&>(obj);
			j["vehicleUuid"] = a.getVehicleUuid();
			j["place"] = a.getPlace();
		}

		static void from_json(const json &j, TeleportToVehiclePlayerObjective &obj) {
			auto &playerObjective = static_cast<PlayerObjective&>(obj);
			adl_serializer<PlayerObjective>::from_json(j, playerObjective);

			j.at("vehicleUuid").get_to(obj.getVehicleUuid());
			j.at("place").get_to(obj.getPlace());
		}
	};

NLOHMANN_JSON_NAMESPACE_END
