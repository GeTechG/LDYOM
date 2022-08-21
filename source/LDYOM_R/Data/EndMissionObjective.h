#pragma once
#include "MissionObjective.h"

class EndMissionObjective final: public virtual MissionObjective {
private:
	friend class boost::serialization::access;

	template <class Archive>
	void serialize(Archive& ar, const unsigned version) {
		ar & boost::serialization::base_object<MissionObjective>(*this);
		ar & showEndTextMission_;
		ar & boost::serialization::make_array(endTextMission_.data(), endTextMission_.size());
		ar & styleTypeEndText_;
		ar & endTextMissionTime_;
		ar & endMissionTune_;
	}

	bool showEndTextMission_ = true;
	std::array<char, TEXT_SIZE> endTextMission_ = { "" };
	int styleTypeEndText_ = 0;
	float endTextMissionTime_ = 2.f;
	int endMissionTune_ = 1;
public:
	EndMissionObjective() = default;
	explicit EndMissionObjective(void* _new);
	~EndMissionObjective() override = default;

	int getTypeCategory() override {
		return 1;
	}

	void draw(Localization & local) override;
	ktwait execute(Scene * scene, Result & result, ktcoro_tasklist & tasklist) override;
};
