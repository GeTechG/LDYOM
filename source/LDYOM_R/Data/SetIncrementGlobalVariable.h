#pragma once
#include "GlobalVariablesService.h"
#include "WorldObjective.h"

enum ConditionActivator {
	None,
	DeadActor,
	PlayerInCar,
};

class SetIncrementGlobalVariable final : virtual public WorldObjective {
private:
	ConditionActivator conditionActivator = None;
	boost::uuids::uuid conditionActivatorUuid{};
	bool increment = false;
	std::string varUuid{};
	GlobalVariableView::Value varValue{GlobalVariableType::Float};

public:
	SetIncrementGlobalVariable() = default;
	explicit SetIncrementGlobalVariable(void *_new);
	~SetIncrementGlobalVariable() override = default;

	int getTypeCategory() override {
		return 9;
	}

	ConditionActivator& getConditionActivator();
	boost::uuids::uuid& getConditionActivatorUuid();
	bool& isIncrement();
	std::string& getVarUuid();
	GlobalVariableView::Value& getVarValue();

	void draw(Localization &local, std::vector<std::string> &listOverlay) override;
	ktwait execute(Scene *scene, Result &result, ktcoro_tasklist &tasklist) override;
};

NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<SetIncrementGlobalVariable> {
		static void to_json(json &j, const SetIncrementGlobalVariable &obj) {
			auto &worldObjective = static_cast<const WorldObjective&>(obj);
			adl_serializer<WorldObjective>::to_json(j, worldObjective);
			auto &a = const_cast<SetIncrementGlobalVariable&>(obj);
			j["conditionActivator"] = a.getConditionActivator();
			j["conditionActivatorUuid"] = a.getConditionActivatorUuid();
			j["increment"] = a.isIncrement();
			j["varUuid"] = a.getVarUuid();
			j["varValue"] = a.getVarValue();
		}

		static void from_json(const json &j, SetIncrementGlobalVariable &obj) {
			auto &worldObjective = static_cast<WorldObjective&>(obj);
			j.get_to(worldObjective);
			j.at("conditionActivator").get_to(obj.getConditionActivator());
			j.at("conditionActivatorUuid").get_to(obj.getConditionActivatorUuid());
			j.at("increment").get_to(obj.isIncrement());
			j.at("varUuid").get_to(obj.getVarUuid());
			j.at("varValue").get_to(obj.getVarValue());
		}
	};

NLOHMANN_JSON_NAMESPACE_END
