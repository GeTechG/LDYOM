#pragma once
#include <array>
#include <CObject.h>
#include <optional>
#include "boost/uuid/uuid.hpp"

#include "INameable.h"
#include "IPositionable.h"
#include "IUuidable.h"
#include "jsonUtils.h"
#include "ObjectiveDependent.h"


class Object final : public ObjectiveDependent, public INameable, public IPositionable, public IUuidable {
private:
	boost::uuids::uuid uuid{};

	std::optional<CObject*> editorObject_;
	std::optional<CObject*> projectObject_;

	std::string name{};
	float pos[3]{};
	CQuaternion rotate;
	std::array<float, 3> scale = {1.f, 1.f, 1.f};
	int modelId{};
	int interiorId = 0;

	std::optional<CObject*> spawnObject();

public:
	Object() = default;
	Object(const char *name, const CVector &pos);
	Object(const Object &other) = default;
	Object& operator=(const Object &other) = default;

	Object copy() const;

	~Object() override;

	boost::uuids::uuid& getUuid() override;

	std::optional<CObject*>& getEditorObject();
	std::optional<CObject*>& getProjectObject();

	CQuaternion& getRotations();
	std::array<float, 3>& getScale();
	int& getModelId();
	int& getInteriorId();

	void updateLocation();

	std::string& getName() override;
	float* getPosition() override;

	void spawnEditorObject();
	void deleteEditorObject();

	void spawnProjectEntity() override;
	void deleteProjectEntity() override;
};

NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<Object> {
		static void to_json(json &j, const Object &obj) {
			auto &base = static_cast<const ObjectiveDependent&>(obj);
			j = base;
			auto &a = const_cast<Object&>(obj);
			j["uuid"] = a.getUuid();
			j["name"] = a.getName();
			j["pos"] = arrayPtrToJson(a.getPosition(), 3);
			j["rotate"] = a.getRotations();
			j["scale"] = a.getScale();
			j["modelId"] = a.getModelId();
			j["interiorId"] = a.getInteriorId();
		}

		static void from_json(const json &j, Object &obj) {
			auto &base = static_cast<ObjectiveDependent&>(obj);
			j.get_to(base);
			j.at("uuid").get_to(obj.getUuid());
			j.at("name").get_to(obj.getName());
			jsonToArrayPtr(j.at("pos"), obj.getPosition(), 3);
			j.at("rotate").get_to(obj.getRotations());
			j.at("scale").get_to(obj.getScale());
			j.at("modelId").get_to(obj.getModelId());
			obj.getInteriorId() = j.contains("interiorId") ? j.at("interiorId").get<int>() : 0;
		}
	};

NLOHMANN_JSON_NAMESPACE_END
