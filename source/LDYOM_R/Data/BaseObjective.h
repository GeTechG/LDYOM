#pragma once
#include <array>
#include <ktcoro_wait.hpp>

#include <nlohmann/json.hpp>

#include "INameable.h"
#include "IUuidable.h"
#include "boost/uuid/uuid_serialize.hpp"
#include "fmt/core.h"
#include "Localization/Localization.h"

#include "jsonUtils.h"
#include "Result.h"


class Result;
class Scene;

class BaseObjective : public INameable, public IUuidable {
protected:
	BaseObjective();
	BaseObjective(void *_new);
	BaseObjective(const BaseObjective &other) = default;

	boost::uuids::uuid uuid;

	std::string name;

public:
	~BaseObjective() override = default;

	boost::uuids::uuid& getUuid() override;

	std::string& getName() override;

	virtual int getCategory() = 0;
	virtual int getTypeCategory() = 0;
	virtual void open() {}
	virtual void close() {}
	virtual void draw(Localization &local, std::vector<std::string> &listOverlay) = 0;
	virtual ktwait execute(Scene *scene, Result &result, ktcoro_tasklist &tasklist) = 0;
};

inline void setObjectiveError(Result &result, BaseObjective &objective, unsigned code, std::string message) {
	result.setError(code, fmt::format("{}\nObjective: {}", message, objective.getName()));
}

NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<BaseObjective> {
		static void to_json(json &j, const BaseObjective &obj) {
			auto &o = const_cast<BaseObjective&>(obj);
			j["uuid"] = o.getUuid();
			j["name"] = o.getName();
			j["category"] = o.getCategory();
			j["typeCategory"] = o.getTypeCategory();
		}

		static void from_json(const json &j, BaseObjective &obj) {
			j.at("uuid").get_to(obj.getUuid());
			j.at("name").get_to(obj.getName());
		}
	};

NLOHMANN_JSON_NAMESPACE_END
