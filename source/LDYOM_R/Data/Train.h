#pragma once
#include <optional>

#include "CTrain.h"
#include "INameable.h"
#include "IPositionable.h"
#include "IUuidable.h"
#include "jsonUtils.h"
#include "ObjectiveDependent.h"


class Train final : public ObjectiveDependent, public INameable, public IPositionable, public IUuidable {
private:
	boost::uuids::uuid uuid_{};

	std::optional<CTrain*> editorTrain_;
	std::optional<CTrain*> projectTrain_;
	std::string name_;
	float pos_[3]{};
	bool rotate_ = false;
	int health_ = 1000;
	int trainType_{};
	float speed_ = 0.0f;
	float cruiseSpeed_ = 0.0f;

	CTrain* spawnTrain();

public:
	Train() = default;
	Train(const char *name, const CVector &pos);
	Train(const Train &other) = default;
	Train& operator=(const Train &other) = default;
	~Train() override;

	Train copy() const;

	boost::uuids::uuid& getUuid() override;

	std::optional<CTrain*>& getEditorTrain();
	std::optional<CTrain*>& getProjectTrain();

	bool& isRotate();
	int& getHealth();
	int& getTrainType();
	float& getSpeed();
	float& getCruiseSpeed();

	void updateLocation();

	std::string& getName() override;
	float* getPosition() override;

	void spawnEditorTrain();
	void deleteEditorTrain();

	void spawnProjectEntity() override;
	void deleteProjectEntity() override;
};


NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<Train> {
		static void to_json(json &j, const Train &obj) {
			auto &objectiveDependent = static_cast<const ObjectiveDependent&>(obj);
			adl_serializer<ObjectiveDependent>::to_json(j, objectiveDependent);
			auto &a = const_cast<Train&>(obj);
			j["uuid"] = a.getUuid();
			j["name"] = a.getName();
			j["pos"] = arrayPtrToJson(a.getPosition(), 3);
			j["rotate"] = a.isRotate();
			j["health"] = a.getHealth();
			j["trainType"] = a.getTrainType();
			j["speed"] = a.getSpeed();
			j["cruiseSpeed"] = a.getCruiseSpeed();
		}

		static void from_json(const json &j, Train &obj) {
			auto &objectiveDependent = static_cast<ObjectiveDependent&>(obj);
			j.get_to(objectiveDependent);
			j.at("uuid").get_to(obj.getUuid());
			j.at("name").get_to(obj.getName());
			jsonToArrayPtr(j.at("pos"), obj.getPosition(), 3);
			j.at("rotate").get_to(obj.isRotate());
			j.at("health").get_to(obj.getHealth());
			j.at("trainType").get_to(obj.getTrainType());
			j.at("speed").get_to(obj.getSpeed());
			j.at("cruiseSpeed").get_to(obj.getCruiseSpeed());
		}
	};

NLOHMANN_JSON_NAMESPACE_END
