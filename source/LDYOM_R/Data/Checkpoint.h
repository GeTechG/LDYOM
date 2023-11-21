#pragma once
#include <array>
#include <CCheckpoint.h>
#include <optional>
#include "boost/uuid/uuid.hpp"

#include "INameable.h"
#include "IPositionable.h"
#include "IUuidable.h"
#include "jsonUtils.h"
#include "ObjectiveDependent.h"
#include "utils.h"


class Checkpoint final : public ObjectiveDependent, public INameable, public IPositionable, public IUuidable {
private:
	boost::uuids::uuid uuid{};

	std::optional<int> editorCheckpoint_;
	std::optional<int> projectCheckpoint_;
	std::optional<int> editorBlip;
	std::optional<int> projectBlip;
	std::optional<unsigned> editorSphere;
	std::optional<unsigned> projectSphere;

	std::string name{};
	std::array<float, 3> pos{};
	int type = 0;
	float radius = 1.f;
	int blipColor = 0;
	std::array<float, 4> color = {1.f, 0.f, 0.f, 0.894f};
	int checkpointType = 0;
	int blipType = 0;
	int blipSprite = 0;
	float angle = 0.f;
	bool rerender = false;

	std::optional<int> spawnCheckpoint() const;

public:
	Checkpoint() = default;
	Checkpoint(const char *name, const CVector &pos);
	Checkpoint(const Checkpoint &other) = default;
	Checkpoint& operator=(const Checkpoint &other) = default;

	Checkpoint copy() const;

	~Checkpoint() override;

	boost::uuids::uuid& getUuid() override;

	std::optional<CCheckpoint*> getEditorCheckpoint();
	std::optional<CCheckpoint*> getProjectCheckpoint();
	std::optional<unsigned>& getEditorSphere();
	std::optional<unsigned>& getProjectSphere();

	void updateLocation();

	std::string& getName() override;
	float* getPosition() override;
	int& getType();
	float& getRadius();
	int& getBlipColor();
	std::array<float, 4>& getColor();
	int& getCheckpointType();
	int& getBlipType();
	int& getBlipSprite();
	float& getAngle();


	void spawnEditorCheckpoint();
	void deleteEditorCheckpoint();

	void spawnProjectEntity() override;
	void deleteProjectEntity() override;

	void spawnEditorBlip();
	void deleteEditorBlip();

	void spawnProjectBlip();
	void deleteProjectBlip();

	void drawSphere();
	bool existProjectEntity();
	CVector getProjectEntityPosition();
};

NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<Checkpoint> {
		static void to_json(json &j, const Checkpoint &obj) {
			auto &base = static_cast<const ObjectiveDependent&>(obj);
			j = base;
			auto &a = const_cast<Checkpoint&>(obj);
			j["uuid"] = a.getUuid();
			j["name"] = a.getName();
			j["pos"] = arrayPtrToJson(a.getPosition(), 3);
			j["type"] = a.getType();
			j["radius"] = a.getRadius();
			j["blipColor"] = a.getBlipColor();
			j["color"] = utils::floatArrayColorToHex(a.getColor());
			j["checkpointType"] = a.getCheckpointType();
			j["blipType"] = a.getBlipType();
			j["blipSprite"] = a.getBlipSprite();
			j["angle"] = a.getAngle();
		}

		static void from_json(const json &j, Checkpoint &obj) {
			auto &base = static_cast<ObjectiveDependent&>(obj);
			j.get_to(base);
			j.at("uuid").get_to(obj.getUuid());
			j.at("name").get_to(obj.getName());
			jsonToArrayPtr(j.at("pos"), obj.getPosition(), 3);
			j.at("type").get_to(obj.getType());
			j.at("radius").get_to(obj.getRadius());
			j.at("blipColor").get_to(obj.getBlipColor());
			obj.getColor() = utils::hexToFloatArrayColor(j.at("color").get<std::string>());
			j.at("checkpointType").get_to(obj.getCheckpointType());
			j.at("blipType").get_to(obj.getBlipType());
			j.at("blipSprite").get_to(obj.getBlipSprite());
			j.at("angle").get_to(obj.getAngle());
		}
	};

NLOHMANN_JSON_NAMESPACE_END
