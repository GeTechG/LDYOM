#pragma once
#include "WorldObjective.h"
#include "glm/ext/scalar_constants.hpp"

class AttachPedToEntity final : virtual public WorldObjective {
private:
	boost::uuids::uuid entityAttachUuid_{};
	boost::uuids::uuid entityTargetUuid_{};
	std::array<float, 3> position{};
	int attachType = 0;
	int targetType = 0;
	int positionType = 0;
	float limitAngleX = glm::pi<float>();
	float limitAngleY = glm::pi<float>();
	int weaponType = 1;

public:
	AttachPedToEntity() = default;
	explicit AttachPedToEntity(const CVector &position);
	~AttachPedToEntity() override = default;

	int getTypeCategory() override {
		return 11;
	}

	void open() override;
	void close() override;

	void draw(Localization &local, std::vector<std::string> &listOverlay) override;
	ktwait execute(Scene *scene, Result &result, ktcoro_tasklist &tasklist) override;

	boost::uuids::uuid& getEntityAttachUuid();
	boost::uuids::uuid& getEntityTargetUuid();
	std::array<float, 3>& getPosition();
	int& getAttachType();
	int& getTargetType();
	int& getPositionType();
	float& getLimitAngleX();
	float& getLimitAngleY();
	int& getWeaponType();
};

NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<AttachPedToEntity> {
		static void to_json(json &j, const AttachPedToEntity &obj) {
			auto &worldObjective = static_cast<const WorldObjective&>(obj);
			adl_serializer<WorldObjective>::to_json(j, worldObjective);
			auto &a = const_cast<AttachPedToEntity&>(obj);
			j["entityAttachUuid"] = a.getEntityAttachUuid();
			j["entityTargetUuid"] = a.getEntityTargetUuid();
			j["position"] = a.getPosition();
			j["attachType"] = a.getAttachType();
			j["targetType"] = a.getTargetType();
			j["positionType"] = a.getPositionType();
			j["limitAngleX"] = a.getLimitAngleX();
			j["limitAngleY"] = a.getLimitAngleY();
			j["weaponType"] = a.getWeaponType();
		}

		static void from_json(const json &j, AttachPedToEntity &obj) {
			auto &worldObjective = static_cast<WorldObjective&>(obj);
			j.get_to(worldObjective);
			j.at("entityAttachUuid").get_to(obj.getEntityAttachUuid());
			j.at("entityTargetUuid").get_to(obj.getEntityTargetUuid());
			j.at("position").get_to(obj.getPosition());
			j.at("attachType").get_to(obj.getAttachType());
			j.at("targetType").get_to(obj.getTargetType());
			j.at("positionType").get_to(obj.getPositionType());
			j.at("limitAngleX").get_to(obj.getLimitAngleX());
			j.at("limitAngleY").get_to(obj.getLimitAngleY());
			j.at("weaponType").get_to(obj.getWeaponType());
		}
	};

NLOHMANN_JSON_NAMESPACE_END
