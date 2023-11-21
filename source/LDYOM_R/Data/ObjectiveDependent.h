#pragma once
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid.hpp>
#include <nlohmann/json.hpp>
#include "jsonUtils.h"


using json = nlohmann::json;

class ObjectiveDependent {
private:
	friend class boost::serialization::access;

protected:
	bool useObjective = true;
	boost::uuids::uuid spawnObjectiveUuid{};
	boost::uuids::uuid deleteObjectiveUuid{};

public:
	ObjectiveDependent() = default;
	ObjectiveDependent(void *new_);
	ObjectiveDependent(const ObjectiveDependent &other) = default;
	ObjectiveDependent(ObjectiveDependent &&other) = default;
	ObjectiveDependent& operator=(const ObjectiveDependent &other) = default;
	ObjectiveDependent& operator=(ObjectiveDependent &&other) = default;

	virtual ~ObjectiveDependent() = default;

	virtual void spawnProjectEntity() = 0;
	virtual void deleteProjectEntity() = 0;

	bool& isUseObjective();
	boost::uuids::uuid& getSpawnObjectiveUuid();
	boost::uuids::uuid& getDeleteObjectiveUuid();
};

NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<ObjectiveDependent> {
		static void to_json(json &j, const ObjectiveDependent &obj) {
			auto &o = const_cast<ObjectiveDependent&>(obj);
			j = json{
				{"useObjective", o.isUseObjective()},
				{"spawnObjectiveUuid", o.getSpawnObjectiveUuid()},
				{"deleteObjectiveUuid", o.getDeleteObjectiveUuid()}
			};
		}

		static void from_json(const json &j, ObjectiveDependent &obj) {
			j.at("useObjective").get_to(obj.isUseObjective());
			j.at("spawnObjectiveUuid").get_to(obj.getSpawnObjectiveUuid());
			j.at("deleteObjectiveUuid").get_to(obj.getDeleteObjectiveUuid());
		}
	};

NLOHMANN_JSON_NAMESPACE_END
