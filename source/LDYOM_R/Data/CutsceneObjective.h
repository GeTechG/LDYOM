#pragma once
#include <CObject.h>
#include <CQuaternion.h>

#include "jsonUtils.h"
#include "WorldObjective.h"


class CutsceneObjective final : virtual public WorldObjective {
private:
	int attachType_ = 0;
	boost::uuids::uuid attachUuid_{};
	int followType_ = 0;
	boost::uuids::uuid followUuid_{};
	std::array<float, 3> position_{};
	CQuaternion rotation_{};
	float xAngle_ = 0.0f;
	std::string text;
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
	std::optional<CObject*> attachFreeCamera;

	std::string gameText;

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
	void close() override;

	int& getAttachType();
	boost::uuids::uuid& getAttachUuid();
	int& getFollowType();
	boost::uuids::uuid& getFollowUuid();
	std::array<float, 3>& getPosition();
	CQuaternion& getRotation();
	float& getXAngle();
	std::string& getText();
	float& getTextTime();
	bool& isWideScreen();
	bool& isAsync();
	bool& isStartFadeOut();
	float& getStartFadeOutTime();
	bool& isEndFadeIn();
	float& getEndFadeInTime();
	bool& isEndCutscene();
	std::string& getGameText();
	int& getMove();
	bool& isLockPlayerControl();
};

NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<CutsceneObjective> {
		static void to_json(json &j, const CutsceneObjective &obj) {
			auto &worldObjective = static_cast<const WorldObjective&>(obj);
			adl_serializer<WorldObjective>::to_json(j, worldObjective);
			auto &a = const_cast<CutsceneObjective&>(obj);
			j["attachType"] = a.getAttachType();
			j["attachUuid"] = a.getAttachUuid();
			j["followType"] = a.getFollowType();
			j["followUuid"] = a.getFollowUuid();
			j["position"] = a.getPosition();
			j["rotation"] = a.getRotation();
			j["xAngle"] = a.getXAngle();
			j["text"] = a.getText();
			j["textTime"] = a.getTextTime();
			j["wideScreen"] = a.isWideScreen();
			j["async"] = a.isAsync();
			j["startFadeOut"] = a.isStartFadeOut();
			j["startFadeOutTime"] = a.getStartFadeOutTime();
			j["endFadeIn"] = a.isEndFadeIn();
			j["endFadeInTime"] = a.getEndFadeInTime();
			j["endCutscene"] = a.isEndCutscene();
			j["lockPlayerControl"] = a.isLockPlayerControl();
			j["move"] = a.getMove();
		}

		static void from_json(const json &j, CutsceneObjective &obj) {
			auto &worldObjective = static_cast<WorldObjective&>(obj);
			j.get_to(worldObjective);
			j.at("attachType").get_to(obj.getAttachType());
			j.at("attachUuid").get_to(obj.getAttachUuid());
			j.at("followType").get_to(obj.getFollowType());
			j.at("followUuid").get_to(obj.getFollowUuid());
			j.at("position").get_to(obj.getPosition());
			j.at("rotation").get_to(obj.getRotation());
			j.at("xAngle").get_to(obj.getXAngle());
			j.at("text").get_to(obj.getText());
			j.at("textTime").get_to(obj.getTextTime());
			j.at("wideScreen").get_to(obj.isWideScreen());
			j.at("async").get_to(obj.isAsync());
			j.at("startFadeOut").get_to(obj.isStartFadeOut());
			j.at("startFadeOutTime").get_to(obj.getStartFadeOutTime());
			j.at("endFadeIn").get_to(obj.isEndFadeIn());
			j.at("endFadeInTime").get_to(obj.getEndFadeInTime());
			j.at("endCutscene").get_to(obj.isEndCutscene());
			j.at("lockPlayerControl").get_to(obj.isLockPlayerControl());
			j.at("move").get_to(obj.getMove());
		}
	};

NLOHMANN_JSON_NAMESPACE_END
