#pragma once
#include <array>
#include <FxSystem_c.h>
#include <optional>
#include "boost/uuid/uuid.hpp"

#include "INameable.h"
#include "IPositionable.h"
#include "IUuidable.h"
#include "jsonUtils.h"
#include "ObjectiveDependent.h"


class Particle final : public ObjectiveDependent, public INameable, public IPositionable, public IUuidable {
private:
	boost::uuids::uuid uuid_{};

	std::optional<int> editorParticle_;
	std::optional<int> projectParticle_;

	std::string name_;
	std::array<float, 3> pos_{};
	CQuaternion rotate;
	std::array<float, 3> scale_ = {1.f, 1.f, 1.f};
	int particleType_{};
	int attachType_ = 0;
	boost::uuids::uuid attachUuid_{};
	int pedBodeId_ = 1;

	std::optional<int> spawnParticle();

public:
	Particle() = default;
	Particle(const char *name, const CVector &pos);

	Particle(const Particle &other) = default;

	Particle& operator=(const Particle &other) = default;

	Particle copy() const;

	~Particle() override;

	boost::uuids::uuid& getUuid() override;

	std::optional<FxSystem_c*> getEditorParticle();
	std::optional<FxSystem_c*> getProjectParticle();

	CQuaternion& getRotations();
	std::array<float, 3>& getScale();
	int& getParticleType();
	int& getAttachType();
	boost::uuids::uuid& getAttachUuid();
	int& getPedBodeId();
	std::optional<int>& getProjectParticleId();

	void updateLocation();

	std::string& getName() override;
	float* getPosition() override;

	void spawnEditorParticle();
	void deleteEditorParticle();

	void spawnProjectEntity() override;
	void deleteProjectEntity() override;
};

NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<Particle> {
		static void to_json(json &j, const Particle &obj) {
			auto &objectiveDependent = static_cast<const ObjectiveDependent&>(obj);
			adl_serializer<ObjectiveDependent>::to_json(j, objectiveDependent);
			auto &a = const_cast<Particle&>(obj);
			j["uuid"] = a.getUuid();
			j["name"] = a.getName();
			j["pos"] = arrayPtrToJson(a.getPosition(), 3);
			j["rotate"] = a.getRotations();
			j["scale"] = a.getScale();
			j["particleType"] = a.getParticleType();
			j["attachType"] = a.getAttachType();
			j["attachUuid"] = a.getAttachUuid();
			j["pedBodeId"] = a.getPedBodeId();
		}

		static void from_json(const json &j, Particle &obj) {
			auto &objectiveDependent = static_cast<ObjectiveDependent&>(obj);
			j.get_to(objectiveDependent);
			j.at("uuid").get_to(obj.getUuid());
			j.at("name").get_to(obj.getName());
			jsonToArrayPtr(j.at("pos"), obj.getPosition(), 3);
			j.at("rotate").get_to(obj.getRotations());
			j.at("scale").get_to(obj.getScale());
			j.at("particleType").get_to(obj.getParticleType());
			j.at("attachType").get_to(obj.getAttachType());
			j.at("attachUuid").get_to(obj.getAttachUuid());
			j.at("pedBodeId").get_to(obj.getPedBodeId());
		}
	};

NLOHMANN_JSON_NAMESPACE_END
