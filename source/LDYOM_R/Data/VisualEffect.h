#pragma once
#include <array>
#include <CVector.h>
#include <ktcoro_wait.hpp>
#include <optional>
#include "boost/uuid/uuid.hpp"

#include "INameable.h"
#include "IPositionable.h"
#include "IUuidable.h"
#include "jsonUtils.h"
#include "ObjectiveDependent.h"
#include "utils.h"


class VisualEffect final : public ObjectiveDependent, public INameable, public IPositionable, public IUuidable {
private:
	boost::uuids::uuid uuid_{};

	std::optional<unsigned> editorVisualEffect_;
	std::optional<unsigned> projectVisualEffect_;

	std::string name_;
	std::array<float, 3> pos_{};
	int type_ = 0;
	float size_ = 1.0f;
	int effectType_ = 0;
	int flare_ = 0;
	std::array<float, 4> color_ = {1.f, 1.f, 1.f, 1.f};
	bool drawing_ = false;
	float angle_ = 0.f;

public:
	VisualEffect() = default;
	VisualEffect(const char *name, const CVector &pos);

	VisualEffect(const VisualEffect &other) = default;
	VisualEffect& operator=(const VisualEffect &other) = default;

	VisualEffect copy() const;

	~VisualEffect() override;

	boost::uuids::uuid& getUuid() override;
	int& getType();
	float& getSize();
	int& getEffectType();
	int& getFlare();
	std::array<float, 4>& getColor();
	bool& isDrawing();
	float& getAngle();

	std::string& getName() override;
	float* getPosition() override;

	void spawnEditorVisualEffect();
	void deleteEditorVisualEffect();

	void spawnProjectEntity() override;
	void deleteProjectEntity() override;

	void drawVisualEffect();
};


NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<VisualEffect> {
		static void to_json(json &j, const VisualEffect &obj) {
			auto &objectiveDependent = static_cast<const ObjectiveDependent&>(obj);
			adl_serializer<ObjectiveDependent>::to_json(j, objectiveDependent);
			auto &a = const_cast<VisualEffect&>(obj);
			j["uuid"] = a.getUuid();
			j["name"] = a.getName();
			j["pos"] = arrayPtrToJson(a.getPosition(), 3);
			j["type"] = a.getType();
			j["size"] = a.getSize();
			j["effectType"] = a.getEffectType();
			j["flare"] = a.getFlare();
			j["color"] = utils::floatArrayColorToHex(a.getColor());
			j["drawing"] = a.isDrawing();
			j["angle"] = a.getAngle();
		}

		static void from_json(const json &j, VisualEffect &obj) {
			auto &objectiveDependent = static_cast<ObjectiveDependent&>(obj);
			j.get_to(objectiveDependent);
			j.at("uuid").get_to(obj.getUuid());
			j.at("name").get_to(obj.getName());
			jsonToArrayPtr(j.at("pos"), obj.getPosition(), 3);
			j.at("type").get_to(obj.getType());
			j.at("size").get_to(obj.getSize());
			j.at("effectType").get_to(obj.getEffectType());
			j.at("flare").get_to(obj.getFlare());
			obj.getColor() = utils::hexToFloatArrayColor(j.at("color").get<std::string>());
			j.at("drawing").get_to(obj.isDrawing());
			j.at("angle").get_to(obj.getAngle());
		}
	};

NLOHMANN_JSON_NAMESPACE_END
