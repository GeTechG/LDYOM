#pragma once
#include "MissionObjective.h"

class StartMissionObjective final : public virtual MissionObjective {
private:
	bool fade_ = false;
	float fadeInTime_ = 1.f;
	float fadeOutTime_ = 1.f;
	bool showNameMission_ = true;
	std::string nameMission_;
	int styleTypeName_ = 1;
	float nameMissionTime_ = 2.f;
	std::array<std::array<int, 9>, 8> groupRelations{};
	int wantedMin = 0;
	int wantedMax = 6;
	bool riot = false;

public:
	StartMissionObjective() = default;
	explicit StartMissionObjective(void *_new);
	~StartMissionObjective() override = default;

	int getTypeCategory() override {
		return 0;
	}

	bool& isFade();
	float& getFadeInTime();
	float& getFadeOutTime();
	bool& isShowNameMission();
	std::string& getNameMission();
	int& getStyleTypeName();
	float& getNameMissionTime();
	std::array<std::array<int, 9>, 8>& getGroupRelations();
	int& getWantedMin();
	int& getWantedMax();
	bool& isRiot();

	void draw(Localization &local, std::vector<std::string> &listOverlay) override;
	ktwait execute(Scene *scene, Result &result, ktcoro_tasklist &tasklist) override;
};

NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<StartMissionObjective> {
		static void to_json(json &j, const StartMissionObjective &obj) {
			auto &missionObjective = static_cast<const MissionObjective&>(obj);
			adl_serializer<MissionObjective>::to_json(j, missionObjective);
			auto &a = const_cast<StartMissionObjective&>(obj);
			j["fade"] = a.isFade();
			j["fadeInTime"] = a.getFadeInTime();
			j["fadeOutTime"] = a.getFadeOutTime();
			j["showNameMission"] = a.isShowNameMission();
			j["nameMission"] = a.getNameMission();
			j["styleTypeName"] = a.getStyleTypeName();
			j["nameMissionTime"] = a.getNameMissionTime();
			j["groupRelations"] = a.getGroupRelations();
			j["wantedMin"] = a.getWantedMin();
			j["wantedMax"] = a.getWantedMax();
			j["riot"] = a.isRiot();
		}

		static void from_json(const json &j, StartMissionObjective &obj) {
			auto &missionObjective = static_cast<MissionObjective&>(obj);
			adl_serializer<MissionObjective>::from_json(j, missionObjective);

			j.at("fade").get_to(obj.isFade());
			j.at("fadeInTime").get_to(obj.getFadeInTime());
			j.at("fadeOutTime").get_to(obj.getFadeOutTime());
			j.at("showNameMission").get_to(obj.isShowNameMission());
			j.at("nameMission").get_to(obj.getNameMission());
			j.at("styleTypeName").get_to(obj.getStyleTypeName());
			j.at("nameMissionTime").get_to(obj.getNameMissionTime());
			j.at("groupRelations").get_to(obj.getGroupRelations());
			j.at("wantedMin").get_to(obj.getWantedMin());
			j.at("wantedMax").get_to(obj.getWantedMax());
			j.at("riot").get_to(obj.isRiot());
		}
	};

NLOHMANN_JSON_NAMESPACE_END
