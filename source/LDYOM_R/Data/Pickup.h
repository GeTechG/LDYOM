#pragma once
#include <array>
#include <CPickup.h>
#include <optional>
#include "jsonUtils.h"
#include "boost/uuid/uuid.hpp"

#include "INameable.h"
#include "IPositionable.h"
#include "IUuidable.h"
#include "ObjectiveDependent.h"

class Pickup final : public ObjectiveDependent, public INameable, public IPositionable, public IUuidable {
private:
	boost::uuids::uuid uuid_{};

	std::optional<int> editorPickup_;
	std::optional<int> projectPickup_;

	std::string name_;
	std::array<float, 3> pos_{};
	int type_ = 0;
	int spawnType_ = 0;
	int weapon_ = 1;
	int ammo_ = 0;
	int modelId_ = 0;

	std::optional<int> spawnPickup(int pickupType);

public:
	Pickup() = default;
	Pickup(const char *name, const CVector &pos);

	Pickup(const Pickup &other) = default;
	Pickup& operator=(const Pickup &other) = default;

	Pickup copy() const;

	~Pickup() override;

	boost::uuids::uuid& getUuid() override;

	std::optional<CPickup*> getEditorPickup();
	std::optional<CPickup*> getProjectPickup();

	int& getModelId();
	int& getType();
	int& getSpawnType();
	int& getWeapon();
	int& getAmmo();
	std::optional<int>& getProjectPickupIndex();
	std::optional<int>& getEditorPickupIndex();

	void updateLocation();

	std::string& getName() override;
	float* getPosition() override;

	void spawnEditorPickup();
	void deleteEditorPickup();

	void spawnProjectEntity() override;
	void deleteProjectEntity() override;
};


NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<Pickup> {
		static void to_json(json &j, const Pickup &obj) {
			auto &objectiveDependent = static_cast<const ObjectiveDependent&>(obj);
			adl_serializer<ObjectiveDependent>::to_json(j, objectiveDependent);
			auto &a = const_cast<Pickup&>(obj);
			j["uuid"] = a.getUuid();
			j["name"] = a.getName();
			j["pos"] = arrayPtrToJson(a.getPosition(), 3);
			j["modelId"] = a.getModelId();
			j["type"] = a.getType();
			j["spawnType"] = a.getSpawnType();
			j["weapon"] = a.getWeapon();
			j["ammo"] = a.getAmmo();
		}

		static void from_json(const json &j, Pickup &obj) {
			auto &objectiveDependent = static_cast<ObjectiveDependent&>(obj);
			j.get_to(objectiveDependent);
			j.at("uuid").get_to(obj.getUuid());
			j.at("name").get_to(obj.getName());
			jsonToArrayPtr(j.at("pos"), obj.getPosition(), 3);
			j.at("modelId").get_to(obj.getModelId());
			j.at("type").get_to(obj.getType());
			j.at("spawnType").get_to(obj.getSpawnType());
			j.at("weapon").get_to(obj.getWeapon());
			j.at("ammo").get_to(obj.getAmmo());
		}
	};

NLOHMANN_JSON_NAMESPACE_END
