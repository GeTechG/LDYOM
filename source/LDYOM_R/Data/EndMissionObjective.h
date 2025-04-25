#pragma once
#include "MissionObjective.h"

class EndMissionObjective final : public virtual MissionObjective {
private:
	bool showEndTextMission_ = true;
	std::string endTextMission_;
	int styleTypeEndText_ = 0;
	float endTextMissionTime_ = 2.f;
	int endMissionTune_ = 1;

public:
	EndMissionObjective() = default;
	explicit EndMissionObjective(void *_new);
	~EndMissionObjective() override = default;

	int getTypeCategory() override {
		return 1;
	}

	bool& isShowEndTextMission();
	std::string& getEndTextMission();
	int& getStyleTypeEndText();
	float& getEndTextMissionTime();
	int& getEndMissionTune();

	void draw(Localization &local, std::vector<std::string> &listOverlay) override;
	ktwait execute(Scene *scene, Result &result, ktcoro_tasklist &tasklist) override;
};

NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<EndMissionObjective> {
		static void to_json(json &j, const EndMissionObjective &obj) {
			auto &missionObjective = static_cast<const MissionObjective&>(obj);
			adl_serializer<MissionObjective>::to_json(j, missionObjective);
			auto &a = const_cast<EndMissionObjective&>(obj);
			j["showEndTextMission"] = a.isShowEndTextMission();
			j["endTextMission"] = a.getEndTextMission();
			j["styleTypeEndText"] = a.getStyleTypeEndText();
			j["endTextMissionTime"] = a.getEndTextMissionTime();
			j["endMissionTune"] = a.getEndMissionTune();
		}

		static void from_json(const json &j, EndMissionObjective &obj) {
			auto &missionObjective = static_cast<MissionObjective&>(obj);
			adl_serializer<MissionObjective>::from_json(j, missionObjective);

			j.at("showEndTextMission").get_to(obj.isShowEndTextMission());
			j.at("endTextMission").get_to(obj.getEndTextMission());
			j.at("styleTypeEndText").get_to(obj.getStyleTypeEndText());
			j.at("endTextMissionTime").get_to(obj.getEndTextMissionTime());
			j.at("endMissionTune").get_to(obj.getEndMissionTune());
		}
	};

NLOHMANN_JSON_NAMESPACE_END
