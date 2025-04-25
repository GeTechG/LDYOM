#pragma once
#include <CVehicle.h>
#include <optional>
#include "boost/uuid/uuid.hpp"

#include "INameable.h"
#include "IPositionable.h"
#include "IUuidable.h"
#include "jsonUtils.h"
#include "ObjectiveDependent.h"
#include "utils.h"


class Vehicle final : public ObjectiveDependent, public INameable, public IPositionable, public IUuidable {
private:
	boost::uuids::uuid uuid_{};

	std::optional<CVehicle*> editorVehicle_;
	std::optional<CVehicle*> projectVehicle_;

	std::string name;
	float pos_[3]{};
	float headingAngle_{};
	int modelId_{};
	bool shouldNotDie_{};
	int health_{};
	bool bulletproof_{};
	bool fireproof_{};
	bool explosionproof_{};
	bool collisionproof_{};
	bool meleeproof_{};
	bool tiresVulnerability_{};
	bool extendedColor_{};
	float primaryColor_[4]{};
	float secondaryColor_[4]{};
	bool locked_{};
	std::vector<std::pair<unsigned char, std::array<float, 4>>> colors_;
	int componentTypeA_{};
	int componentTypeB_{};
	int numberplateCity_{};
	std::string numberplate_;
	std::array<int, 15> upgrades_ = {-1};
	int paintjob_ = -1;
	std::array<float, 17> damagesComponents_{};
	float extraPartsAngle_ = 0.f;
	bool isLightsOn_ = false;
	std::array<float, 6> openDoorsRation_ = {};
	float dirtyLevel_ = 0.f;
	bool heavy_ = false;
	int interiorId = 0;
	bool isGameColorsMode = false;
	unsigned char primaryColorId = 0;
	unsigned char secondaryColorId = 0;
	unsigned char tertiaryColorId = 0;
	unsigned char quaternaryColorId = 0;

public:
	Vehicle() = default;
	Vehicle(const char *name, const CVector &pos, float headingAngle);

	Vehicle(const Vehicle &other) = default;

	Vehicle& operator=(const Vehicle &other) = default;

	Vehicle copy() const;

	~Vehicle() override;

	boost::uuids::uuid& getUuid() override;

	std::optional<CVehicle*>& getEditorVehicle();
	std::optional<CVehicle*>& getProjectVehicle();

	int& getModelId();
	bool& isShouldNotDie();
	int& getHealth();
	bool& isBulletproof();
	bool& isFireproof();
	bool& isExplosionproof();
	bool& isCollisionproof();
	bool& isMeleeproof();
	bool& isTiresVulnerability();
	bool& isExtendedColor();
	bool& isLocked();
	std::vector<std::pair<unsigned char, std::array<float, 4>>>& getColors();
	int& getComponentTypeA();
	int& getComponentTypeB();
	int& getNumberplateCity();
	float* getPrimaryColor();
	float* getSecondaryColor();
	std::string& getNumberplate();
	std::array<int, 15>& getUpgrades();
	int& getPaintjob();
	std::array<float, 17>& getDamagesComponents();
	float& getExtraPartsAngle();
	bool& isIsLightsOn();
	std::array<float, 6>& getOpenDoorsRation();
	float& getDirtyLevel();
	bool& isHeavy();
	int& getInteriorId();
	bool& isIsGameColorsMode();
	unsigned char& getPrimaryColorId();
	unsigned char& getSecondaryColorId();
	unsigned char& getTertiaryColorId();
	unsigned char& getQuaternaryColorId();

	void updateLocation() const;

	std::string& getName() override;
	float* getPosition() override;

	float& getHeadingAngle();

	void setEditorPrimaryColor();
	void setEditorSecondaryColor();

	void spawnEditorVehicle(bool recolor = false);
	void deleteEditorVehicle();

	CVehicle* spawnVehicle(bool recolor);
	void spawnProjectEntity() override;
	void deleteProjectEntity() override;

	void recolorVehicle(bool recolor, CVehicle *newVehicle);
	void takeUpgrades();
	void restoreUpgrades(CVehicle *newVehicle, bool recolor);

	bool isRecolorBanned() const;
};


NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<Vehicle> {
		static void to_json(json &j, const Vehicle &obj) {
			auto &objectiveDependent = static_cast<const ObjectiveDependent&>(obj);
			adl_serializer<ObjectiveDependent>::to_json(j, objectiveDependent);
			auto &a = const_cast<Vehicle&>(obj);
			j["uuid"] = a.getUuid();
			j["name"] = a.getName();
			j["pos"] = arrayPtrToJson(a.getPosition(), 3);
			j["headingAngle"] = a.getHeadingAngle();
			j["modelId"] = a.getModelId();
			j["shouldNotDie"] = a.isShouldNotDie();
			j["health"] = a.getHealth();
			j["bulletproof"] = a.isBulletproof();
			j["fireproof"] = a.isFireproof();
			j["explosionproof"] = a.isExplosionproof();
			j["collisionproof"] = a.isCollisionproof();
			j["meleeproof"] = a.isMeleeproof();
			j["tiresVulnerability"] = a.isTiresVulnerability();
			j["extendedColor"] = a.isExtendedColor();
			{
				std::array<float, 4> primaryColor{};
				std::copy_n(a.getPrimaryColor(), 4, primaryColor.begin());
				j["primaryColor"] = utils::floatArrayColorToHex(primaryColor);
			}
			{
				std::array<float, 4> secondaryColor{};
				std::copy_n(a.getSecondaryColor(), 4, secondaryColor.begin());
				j["secondaryColor"] = utils::floatArrayColorToHex(secondaryColor);
			}
			j["locked"] = a.isLocked();
			j["colors"] = json::array();
			std::ranges::transform(a.getColors(), std::back_inserter(j["colors"]),
			                       [](const auto &pair) {
				                       return std::make_pair(std::to_string(pair.first),
				                                             utils::floatArrayColorToHex(pair.second));
			                       });
			j["componentTypeA"] = a.getComponentTypeA();
			j["componentTypeB"] = a.getComponentTypeB();
			j["numberplateCity"] = a.getNumberplateCity();
			j["numberplate"] = a.getNumberplate();
			j["upgrades"] = a.getUpgrades();
			j["paintjob"] = a.getPaintjob();
			j["damagesComponents"] = a.getDamagesComponents();
			j["extraPartsAngle"] = a.getExtraPartsAngle();
			j["isLightsOn"] = a.isIsLightsOn();
			j["openDoorsRation"] = a.getOpenDoorsRation();
			j["dirtyLevel"] = a.getDirtyLevel();
			j["heavy"] = a.isHeavy();
			j["interiorId"] = a.getInteriorId();
			j["isGameColorsMode"] = a.isIsGameColorsMode();
			j["primaryColorId"] = a.getPrimaryColorId();
			j["secondaryColorId"] = a.getSecondaryColorId();
			j["tertiaryColorId"] = a.getTertiaryColorId();
			j["quaternaryColorId"] = a.getQuaternaryColorId();
		}

		static void from_json(const json &j, Vehicle &obj) {
			auto &objectiveDependent = static_cast<ObjectiveDependent&>(obj);
			j.get_to(objectiveDependent);
			j.at("uuid").get_to(obj.getUuid());
			j.at("name").get_to(obj.getName());
			jsonToArrayPtr(j.at("pos"), obj.getPosition(), 3);
			j.at("headingAngle").get_to(obj.getHeadingAngle());
			j.at("modelId").get_to(obj.getModelId());
			j.at("shouldNotDie").get_to(obj.isShouldNotDie());
			j.at("health").get_to(obj.getHealth());
			j.at("bulletproof").get_to(obj.isBulletproof());
			j.at("fireproof").get_to(obj.isFireproof());
			j.at("explosionproof").get_to(obj.isExplosionproof());
			j.at("collisionproof").get_to(obj.isCollisionproof());
			j.at("meleeproof").get_to(obj.isMeleeproof());
			j.at("tiresVulnerability").get_to(obj.isTiresVulnerability());
			j.at("extendedColor").get_to(obj.isExtendedColor());
			{
				auto primaryColor = utils::hexToFloatArrayColor(j.at("primaryColor").get<std::string>());
				std::copy_n(primaryColor.begin(), 4, obj.getPrimaryColor());
			}
			{
				auto secondaryColor = utils::hexToFloatArrayColor(j.at("secondaryColor").get<std::string>());
				std::copy_n(secondaryColor.begin(), 4, obj.getSecondaryColor());
			}
			j.at("locked").get_to(obj.isLocked());
			for (const auto &value : j.at("colors")) {
				auto [id, color] = value.get<std::pair<std::string, std::string>>();
				obj.getColors().emplace_back(std::stoi(id), utils::hexToFloatArrayColor(color));
			}
			j.at("componentTypeA").get_to(obj.getComponentTypeA());
			j.at("componentTypeB").get_to(obj.getComponentTypeB());
			j.at("numberplateCity").get_to(obj.getNumberplateCity());
			j.at("numberplate").get_to(obj.getNumberplate());
			{
				if (j.contains("upgrades"))
					j.at("upgrades").get_to(obj.getUpgrades());
				else
					obj.getUpgrades().fill(-1);
			}
			obj.getPaintjob() = j.contains("paintjob") ? j.at("paintjob").get<int>() : -1;
			{
				if (j.contains("damagesComponents"))
					j.at("damagesComponents").get_to(obj.getDamagesComponents());
				else
					obj.getDamagesComponents().fill(0.f);
			}
			obj.getExtraPartsAngle() = j.contains("extraPartsAngle") ? j.at("extraPartsAngle").get<float>() : 0.f;
			obj.isIsLightsOn() = j.contains("isLightsOn") ? j.at("isLightsOn").get<bool>() : false;
			{
				if (j.contains("openDoorsRation"))
					j.at("openDoorsRation").get_to(obj.getOpenDoorsRation());
				else
					obj.getOpenDoorsRation().fill(0.f);
			}
			obj.getDirtyLevel() = j.contains("dirtyLevel") ? j.at("dirtyLevel").get<float>() : 0.f;
			obj.isHeavy() = j.contains("heavy") ? j.at("heavy").get<bool>() : false;
			obj.getInteriorId() = j.contains("interiorId") ? j.at("interiorId").get<int>() : 0;
			obj.isIsGameColorsMode() = j.contains("isGameColorsMode") ? j.at("isGameColorsMode").get<bool>() : false;
			obj.getPrimaryColorId() = j.contains("primaryColorId") ? j.at("primaryColorId").get<unsigned char>() : 0;
			obj.getSecondaryColorId() = j.contains("secondaryColorId")
				                            ? j.at("secondaryColorId").get<unsigned char>()
				                            : 0;
			obj.getTertiaryColorId() = j.contains("tertiaryColorId") ? j.at("tertiaryColorId").get<unsigned char>() : 0;
			obj.getQuaternaryColorId() = j.contains("quaternaryColorId")
				                             ? j.at("quaternaryColorId").get<unsigned char>()
				                             : 0;
		}
	};

NLOHMANN_JSON_NAMESPACE_END
