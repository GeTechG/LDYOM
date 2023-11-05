#pragma once
#include "MissionObjective.h"

class StartMissionObjective final: public virtual MissionObjective {
private:
	friend class boost::serialization::access;

	template <class Archive>
	void serialize(Archive& ar, const unsigned version) {
		ar & boost::serialization::base_object<MissionObjective>(*this);
		ar & fade_;
		ar & fadeInTime_;
		ar & fadeOutTime_;
		ar & showNameMission_;
		ar & boost::serialization::make_array(nameMission_.data(), nameMission_.size());
		ar & styleTypeName_;
		ar & nameMissionTime_;
		ar & groupRelations;
		ar & wantedMin;
		ar & wantedMax;
		ar & riot;
	}

	bool fade_ = false;
	float fadeInTime_ = 1.f;
	float fadeOutTime_ = 1.f;
	bool showNameMission_ = true;
	std::array<char, TEXT_SIZE> nameMission_ = {""};
	int styleTypeName_ = 1;
	float nameMissionTime_ = 2.f;
	std::array<std::array<int, 9>, 8> groupRelations{};
	int wantedMin = 0;
	int wantedMax = 6;
	bool riot = false;
public:
	StartMissionObjective() = default;
	explicit StartMissionObjective(void* _new);
	~StartMissionObjective() override = default;

	int getTypeCategory() override {
		return 0;
	}

	void draw(Localization &local, std::vector<std::string> &listOverlay) override;
	ktwait execute(Scene * scene, Result & result, ktcoro_tasklist & tasklist) override;
};
