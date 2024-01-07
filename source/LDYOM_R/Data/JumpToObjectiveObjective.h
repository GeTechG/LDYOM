#pragma once
#include "GlobalVariablesService.h"
#include "SceneObjective.h"

class JumpToObjectiveObjective final : public virtual SceneObjective {
private:
	boost::uuids::uuid goToObjectiveUuid{};
	boost::uuids::uuid goToObjectiveUuidElse{};
	bool condition = true;
	int conditionType = 0;
	std::string varUuid{};
	GlobalVariableView::Value varValue{GlobalVariableType::Float};

public:
	JumpToObjectiveObjective() = default;
	explicit JumpToObjectiveObjective(void *_new);
	~JumpToObjectiveObjective() override = default;

	int getTypeCategory() override {
		return 2;
	}

	boost::uuids::uuid& getGoToObjectiveUuid();
	boost::uuids::uuid& getGoToObjectiveUuidElse();
	bool& isCondition();
	int& getConditionType();
	std::string& getVarUuid();
	GlobalVariableView::Value& getVarValue();

	void draw(Localization &local, std::vector<std::string> &listOverlay) override;
	ktwait execute(Scene *scene, Result &result, ktcoro_tasklist &tasklist) override;
};

NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<JumpToObjectiveObjective> {
		static void to_json(json &j, const JumpToObjectiveObjective &obj) {
			auto &sceneObjective = static_cast<const SceneObjective&>(obj);
			adl_serializer<SceneObjective>::to_json(j, sceneObjective);

			auto &a = const_cast<JumpToObjectiveObjective&>(obj);
			j["goToObjectiveUuid"] = a.getGoToObjectiveUuid();
			j["goToObjectiveUuidElse"] = a.getGoToObjectiveUuidElse();
			j["condition"] = a.isCondition();
			j["conditionType"] = a.getConditionType();
			j["varUuid"] = a.getVarUuid();
			j["varValue"] = a.getVarValue();
		}

		static void from_json(const json &j, JumpToObjectiveObjective &obj) {
			auto &sceneObjective = static_cast<SceneObjective&>(obj);
			j.get_to(sceneObjective);

			j.at("goToObjectiveUuid").get_to(obj.getGoToObjectiveUuid());
			j.at("goToObjectiveUuidElse").get_to(obj.getGoToObjectiveUuidElse());
			j.at("condition").get_to(obj.isCondition());
			j.at("conditionType").get_to(obj.getConditionType());
			j.at("varUuid").get_to(obj.getVarUuid());
			j.at("varValue").get_to(obj.getVarValue());
		}
	};

NLOHMANN_JSON_NAMESPACE_END
