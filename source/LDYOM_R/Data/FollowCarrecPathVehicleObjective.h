#pragma once
#include "VehicleObjective.h"

class FollowCarrecPathVehicleObjective final : public virtual VehicleObjective {
private:
	boost::uuids::uuid pathUuid_{};
	bool useAI = false;
	bool looped_ = false;
	float time_ = 0;
	bool waitEnd_ = false;

public:
	FollowCarrecPathVehicleObjective() = default;
	explicit FollowCarrecPathVehicleObjective(void *_new);
	~FollowCarrecPathVehicleObjective() override = default;

	int getTypeCategory() override {
		return 4;
	}

	void draw(Localization &local, std::vector<std::string> &listOverlay) override;
	ktwait execute(Scene *scene, Vehicle *vehicle, Result &result, ktcoro_tasklist &tasklist) override;

	boost::uuids::uuid& getPathUuid();
	bool& isUseAI();
	bool& isLooped();
	float& getTime();
	bool& isWaitEnd();
};

NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<FollowCarrecPathVehicleObjective> {
		static void to_json(json &j, const FollowCarrecPathVehicleObjective &obj) {
			auto &vehicleObjective = static_cast<const VehicleObjective&>(obj);
			adl_serializer<VehicleObjective>::to_json(j, vehicleObjective);

			// Add serialization for FollowCarrecPathVehicleObjective specific members
			auto &a = const_cast<FollowCarrecPathVehicleObjective&>(obj);
			j["pathUuid"] = uuidToJson(a.getPathUuid());
			j["useAI"] = a.isUseAI();
			j["looped"] = a.isLooped();
			j["time"] = a.getTime();
			j["waitEnd"] = a.isWaitEnd();
		}

		static void from_json(const json &j, FollowCarrecPathVehicleObjective &obj) {
			auto &vehicleObjective = static_cast<VehicleObjective&>(obj);
			j.get_to(vehicleObjective);

			// Add deserialization for FollowCarrecPathVehicleObjective specific members
			jsonToUuid(j.at("pathUuid"), obj.getPathUuid());
			j.at("useAI").get_to(obj.isUseAI());
			j.at("looped").get_to(obj.isLooped());
			j.at("time").get_to(obj.getTime());
			j.at("waitEnd").get_to(obj.isWaitEnd());
		}
	};

NLOHMANN_JSON_NAMESPACE_END
