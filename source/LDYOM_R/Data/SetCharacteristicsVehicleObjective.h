#pragma once
#include "VehicleObjective.h"

class SetCharacteristicsVehicleObjective : virtual public VehicleObjective {
private:
	bool bulletproof_{};
	bool fireproof_{};
	bool explosionproof_{};
	bool collisionproof_{};
	bool meleeproof_{};
	bool tiresVulnerability_{};
	bool locked_{};
	float extraPartsAngle_ = 0.f;
	bool isLightsOn_ = false;
	std::array<float, 6> openDoorsRation_ = {};
	float dirtyLevel_ = 0.f;

public:
	SetCharacteristicsVehicleObjective() = default;
	explicit SetCharacteristicsVehicleObjective(void *_new);
	~SetCharacteristicsVehicleObjective() override = default;

	int getTypeCategory() override {
		return 3;
	}

	bool& isBulletproof();
	bool& isFireproof();
	bool& isExplosionproof();
	bool& isCollisionproof();
	bool& isMeleeproof();
	bool& isTiresVulnerability();
	bool& isLocked();
	float& getExtraPartsAngle();
	bool& isIsLightsOn();
	std::array<float, 6>& getOpenDoorsRation();
	float& getDirtyLevel();


	void draw(Localization &local, std::vector<std::string> &listOverlay) override;
	ktwait execute(Scene *scene, Vehicle *vehicle, Result &result, ktcoro_tasklist &tasklist) override;
	void open() override;
	void close() override;
};


NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<SetCharacteristicsVehicleObjective> {
		static void to_json(json &j, const SetCharacteristicsVehicleObjective &obj) {
			auto &vehicleObjective = static_cast<const VehicleObjective&>(obj);
			adl_serializer<VehicleObjective>::to_json(j, vehicleObjective);
			auto &a = const_cast<SetCharacteristicsVehicleObjective&>(obj);
			j["bulletproof"] = a.isBulletproof();
			j["fireproof"] = a.isFireproof();
			j["explosionproof"] = a.isExplosionproof();
			j["collisionproof"] = a.isCollisionproof();
			j["meleeproof"] = a.isMeleeproof();
			j["tiresVulnerability"] = a.isTiresVulnerability();
			j["locked"] = a.isLocked();
			j["extraPartsAngle"] = a.getExtraPartsAngle();
			j["isLightsOn"] = a.isIsLightsOn();
			j["openDoorsRation"] = a.getOpenDoorsRation();
			j["dirtyLevel"] = a.getDirtyLevel();
		}

		static void from_json(const json &j, SetCharacteristicsVehicleObjective &obj) {
			auto &vehicleObjective = static_cast<VehicleObjective&>(obj);
			j.get_to(vehicleObjective);

			j.at("bulletproof").get_to(obj.isBulletproof());
			j.at("fireproof").get_to(obj.isFireproof());
			j.at("explosionproof").get_to(obj.isExplosionproof());
			j.at("collisionproof").get_to(obj.isCollisionproof());
			j.at("meleeproof").get_to(obj.isMeleeproof());
			j.at("tiresVulnerability").get_to(obj.isTiresVulnerability());
			j.at("locked").get_to(obj.isLocked());
			j.at("extraPartsAngle").get_to(obj.getExtraPartsAngle());
			j.at("isLightsOn").get_to(obj.isIsLightsOn());
			j.at("openDoorsRation").get_to(obj.getOpenDoorsRation());
			j.at("dirtyLevel").get_to(obj.getDirtyLevel());
		}
	};

NLOHMANN_JSON_NAMESPACE_END
