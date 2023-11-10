#pragma once
#include <CObject.h>
#include <CQuaternion.h>

#include "WorldObjective.h"


class CutsceneObjective final : virtual public WorldObjective {
private:
	friend class boost::serialization::access;

	template <class Archive>
	void serialize(Archive &ar, const unsigned version) {
		ar & boost::serialization::base_object<WorldObjective>(*this);
		ar & attachType_;
		ar & attachUuid_;
		ar & followType_;
		ar & followUuid_;
		ar & boost::serialization::make_array(position_.data(), position_.size());
		ar & rotation_;
		ar & xAngle_;
		ar & boost::serialization::make_array(text_.data(), text_.size());
		ar & textTime_;
		ar & wideScreen_;
		ar & async_;
		ar & startFadeOut;
		ar & startFadeOutTime;
		ar & endFadeIn;
		ar & endFadeInTime;
		ar & endCutscene_;
		ar & lockPlayerControl_;
		ar & move;
	}

	int attachType_ = 0;
	boost::uuids::uuid attachUuid_{};
	int followType_ = 0;
	boost::uuids::uuid followUuid_{};
	std::array<float, 3> position_{};
	CQuaternion rotation_{};
	float xAngle_ = 0.0f;
	std::array<char, TEXT_SIZE> text_ = {""};
	float textTime_ = 1.f;
	int move = 0;
	bool wideScreen_ = false;
	bool async_ = false;
	bool startFadeOut = false;
	float startFadeOutTime = 1.f;
	bool endFadeIn = false;
	float endFadeInTime = 1.f;
	bool endCutscene_ = true;
	bool lockPlayerControl_ = true;

	std::array<char, TEXT_SIZE> gameText_ = {""};

	void updateLocation();

public:
	CutsceneObjective() = default;
	explicit CutsceneObjective(const CVector &position, const CQuaternion &rotation);
	~CutsceneObjective() override = default;

	int getTypeCategory() override {
		return 1;
	}

	void draw(Localization &local, std::vector<std::string> &listOverlay) override;
	ktwait execute(Scene *scene, Result &result, ktcoro_tasklist &tasklist) override;

	void open() override;

	int& getAttachType();
	boost::uuids::uuid& getAttachUuid();
	int& getFollowType();
	boost::uuids::uuid& getFollowUuid();
	std::array<float, 3>& getPosition();
	CQuaternion& getRotation();
	float& getXAngle();
	std::array<char, TEXT_SIZE>& getText();
	float& getTextTime();
	bool& isWideScreen();
	bool& isAsync();
	bool& isStartFadeOut();
	float& getStartFadeOutTime();
	bool& isEndFadeIn();
	float& getEndFadeInTime();
	bool& isEndCutscene();
	std::array<char, TEXT_SIZE>& getGameText();
	int getMove() const;
};
