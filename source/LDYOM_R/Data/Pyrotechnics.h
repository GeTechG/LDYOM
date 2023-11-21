#pragma once
#include <array>
#include <CObject.h>
#include <optional>
#include "jsonUtils.h"
#include "boost/uuid/uuid.hpp"

#include "INameable.h"
#include "IPositionable.h"
#include "IUuidable.h"
#include "ObjectiveDependent.h"

class Pyrotechnics final : public ObjectiveDependent, public INameable, public IPositionable, public IUuidable {
private:
	boost::uuids::uuid uuid_{};

	std::optional<CObject*> editorExplosionObject_;
	std::optional<int> editorFire_;
	std::optional<int> projectFire_;

	std::string name_;
	std::array<float, 3> pos_{};
	int type_ = 0;
	int typeExplosion_ = 0;
	int sizeFire_ = 0;
	int propagationFire_ = 0;


	std::optional<int> spawnFire(bool editor = false);

public:
	Pyrotechnics() = default;
	Pyrotechnics(const char *name, const CVector &pos);

	Pyrotechnics(const Pyrotechnics &other) = default;

	Pyrotechnics& operator=(const Pyrotechnics &other) = default;

	Pyrotechnics copy() const;

	~Pyrotechnics() override;

	boost::uuids::uuid& getUuid() override;

	std::optional<CFire*> getEditorFire();
	std::optional<CFire*> getProjectFire();

	int& getType();
	int& getTypeExplosion();
	int& getSizeFire();
	int& getPropagationFire();
	std::optional<int>& getProjectFireId();

	void updateLocation();

	std::string& getName() override;
	float* getPosition() override;

	void spawnEditorPyrotechnics();
	void deleteEditorPyrotechnics();

	void spawnProjectEntity() override;
	void deleteProjectEntity() override;
};


NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<Pyrotechnics> {
		static void to_json(json &j, const Pyrotechnics &obj) {
			auto &objectiveDependent = static_cast<const ObjectiveDependent&>(obj);
			adl_serializer<ObjectiveDependent>::to_json(j, objectiveDependent);
			auto &a = const_cast<Pyrotechnics&>(obj);
			j["uuid"] = a.getUuid();
			j["name"] = a.getName();
			j["pos"] = arrayPtrToJson(a.getPosition(), 3);
			j["type"] = a.getType();
			j["typeExplosion"] = a.getTypeExplosion();
			j["sizeFire"] = a.getSizeFire();
			j["propagationFire"] = a.getPropagationFire();
		}

		static void from_json(const json &j, Pyrotechnics &obj) {
			auto &objectiveDependent = static_cast<ObjectiveDependent&>(obj);
			j.get_to(objectiveDependent);
			j.at("uuid").get_to(obj.getUuid());
			j.at("name").get_to(obj.getName());
			jsonToArrayPtr(j.at("pos"), obj.getPosition(), 3);
			j.at("type").get_to(obj.getType());
			j.at("typeExplosion").get_to(obj.getTypeExplosion());
			j.at("sizeFire").get_to(obj.getSizeFire());
			j.at("propagationFire").get_to(obj.getPropagationFire());
		}
	};

NLOHMANN_JSON_NAMESPACE_END
