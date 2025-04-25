#pragma once
#include "VehicleObjective.h"

class FollowPathVehicleObjective final : public virtual VehicleObjective {
private:
	int driveType_ = 0;
	int pathType_ = 0;
	float executeTime_ = 1.f;
	std::vector<std::array<float, 3>> path_;
	bool waitEnd_ = false;

public:
	FollowPathVehicleObjective() = default;
	explicit FollowPathVehicleObjective(void *_new);
	~FollowPathVehicleObjective() override = default;

	int getTypeCategory() override {
		return 2;
	}

	void draw(Localization &local, std::vector<std::string> &listOverlay) override;
	ktwait execute(Scene *scene, Vehicle *vehicle, Result &result, ktcoro_tasklist &tasklist) override;

	int& getDriveType();
	int& getPathType();
	float& getExecuteTime();
	std::vector<std::array<float, 3>>& getPath();
	bool& isWaitEnd();
};

NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<FollowPathVehicleObjective> {
		static void to_json(json &j, const FollowPathVehicleObjective &obj) {
			auto &vehicleObjective = static_cast<const VehicleObjective&>(obj);
			adl_serializer<VehicleObjective>::to_json(j, vehicleObjective);

			// Add serialization for FollowPathVehicleObjective specific members
			auto &a = const_cast<FollowPathVehicleObjective&>(obj);
			j["driveType"] = a.getDriveType();
			j["pathType"] = a.getPathType();
			j["executeTime"] = a.getExecuteTime();
			j["path"] = a.getPath();
			j["waitEnd"] = a.isWaitEnd();
		}

		static void from_json(const json &j, FollowPathVehicleObjective &obj) {
			auto &vehicleObjective = static_cast<VehicleObjective&>(obj);
			j.get_to(vehicleObjective);

			// Add deserialization for FollowPathVehicleObjective specific members
			j.at("driveType").get_to(obj.getDriveType());
			j.at("pathType").get_to(obj.getPathType());
			j.at("executeTime").get_to(obj.getExecuteTime());
			j.at("path").get_to(obj.getPath());
			j.at("waitEnd").get_to(obj.isWaitEnd());
		}
	};

NLOHMANN_JSON_NAMESPACE_END
