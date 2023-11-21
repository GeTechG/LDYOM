#pragma once
#include "SceneObjective.h"

class JumpToObjectiveObjective final : public virtual SceneObjective {
private:
	boost::uuids::uuid goToObjectiveUuid_{};
	bool condition_ = true;
	int conditionType_ = 0;
	int varId_ = 1;
	float conditionValueNumber_ = 0.f;
	bool conditionValueBoolean_ = false;

public:
	JumpToObjectiveObjective() = default;
	explicit JumpToObjectiveObjective(void *_new);
	~JumpToObjectiveObjective() override = default;

	int getTypeCategory() override {
		return 2;
	}

	boost::uuids::uuid& getGoToObjectiveUuid();
	bool& isCondition();
	int& getConditionType();
	int& getVarId();
	float& getConditionValueNumber();
	bool& isConditionValueBoolean();

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
			j["condition"] = a.isCondition();
			j["conditionType"] = a.getConditionType();
			j["varId"] = a.getVarId();
			j["conditionValueNumber"] = a.getConditionValueNumber();
			j["conditionValueBoolean"] = a.isConditionValueBoolean();
		}

		static void from_json(const json &j, JumpToObjectiveObjective &obj) {
			auto &sceneObjective = static_cast<SceneObjective&>(obj);
			j.get_to(sceneObjective);

			j.at("goToObjectiveUuid").get_to(obj.getGoToObjectiveUuid());
			j.at("condition").get_to(obj.isCondition());
			j.at("conditionType").get_to(obj.getConditionType());
			j.at("varId").get_to(obj.getVarId());
			j.at("conditionValueNumber").get_to(obj.getConditionValueNumber());
			j.at("conditionValueBoolean").get_to(obj.isConditionValueBoolean());
		}
	};

NLOHMANN_JSON_NAMESPACE_END
