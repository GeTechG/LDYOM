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

	void recolorVehicle(bool recolor, CVehicle *newVehicle);
	CVehicle* spawnVehicle(bool recolor);

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

	void updateLocation() const;

	std::string& getName() override;
	float* getPosition() override;

	float& getHeadingAngle();

	void setEditorPrimaryColor();
	void setEditorSecondaryColor();

	void spawnEditorVehicle(bool recolor = false);
	void deleteEditorVehicle();

	void spawnProjectEntity() override;
	void deleteProjectEntity() override;
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
		}
	};

NLOHMANN_JSON_NAMESPACE_END
