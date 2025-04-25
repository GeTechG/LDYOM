#pragma once
#include "MathCondition.h"
#include "WorldObjective.h"

class AddTimerObjective final : public virtual WorldObjective {
private:
	int typeTimer_ = 0;
	bool backward_ = false;
	MathCondition compareType_ = Equal;
	int compareValue_ = 0;
	int startTime_ = 0;
	std::string text;

	std::string gameText;

public:
	AddTimerObjective() = default;
	explicit AddTimerObjective(void *_new);
	~AddTimerObjective() override = default;

	int getTypeCategory() override {
		return 7;
	}

	void draw(Localization &local, std::vector<std::string> &listOverlay) override;
	ktwait execute(Scene *scene, Result &result, ktcoro_tasklist &tasklist) override;

	int& getTypeTimer();
	MathCondition& getCompareType();
	int& getCompareValue();
	bool& isBackward();
	int& getStartTime();
	std::string& getText();
};

NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<AddTimerObjective> {
		static void to_json(json &j, const AddTimerObjective &obj) {
			auto &worldObjective = static_cast<const WorldObjective&>(obj);
			adl_serializer<WorldObjective>::to_json(j, worldObjective);
			auto &a = const_cast<AddTimerObjective&>(obj);
			j["typeTimer"] = a.getTypeTimer();
			j["backward"] = a.isBackward();
			j["compareType"] = a.getCompareType();
			j["compareValue"] = a.getCompareValue();
			j["startTime"] = a.getStartTime();
			j["text"] = a.getText();
		}

		static void from_json(const json &j, AddTimerObjective &obj) {
			auto &worldObjective = static_cast<WorldObjective&>(obj);
			j.get_to(worldObjective);
			j.at("typeTimer").get_to(obj.getTypeTimer());
			j.at("backward").get_to(obj.isBackward());
			j.at("compareType").get_to(obj.getCompareType());
			j.at("compareValue").get_to(obj.getCompareValue());
			j.at("startTime").get_to(obj.getStartTime());
			j.at("text").get_to(obj.getText());
		}
	};

NLOHMANN_JSON_NAMESPACE_END
