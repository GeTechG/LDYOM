#pragma once
#include <CObject.h>
#include <CQuaternion.h>

#include "jsonUtils.h"
#include "WorldObjective.h"


class PlayCameraPathObjective final : virtual public WorldObjective {
private:
	boost::uuids::uuid cameraPathUuid_{};
	std::string text;
	float textTime_ = 1.f;
	bool async_ = false;
	bool startFadeOut = false;
	float startFadeOutTime = 1.f;
	bool endFadeIn = false;
	float endFadeInTime = 1.f;
	bool lockPlayerControl_ = true;

	std::string gameText;

public:
	PlayCameraPathObjective() = default;
	explicit PlayCameraPathObjective(void *_new);
	~PlayCameraPathObjective() override = default;

	int getTypeCategory() override {
		return 10;
	}

	void draw(Localization &local, std::vector<std::string> &listOverlay) override;
	ktwait execute(Scene *scene, Result &result, ktcoro_tasklist &tasklist) override;

	boost::uuids::uuid& getCameraPathUuid();
	std::string& getText();
	float& getTextTime();
	bool& isAsync();
	bool& isStartFadeOut();
	float& getStartFadeOutTime();
	bool& isEndFadeIn();
	float& getEndFadeInTime();
	bool& isLockPlayerControl();
	std::string& getGameText();
};

NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<PlayCameraPathObjective> {
		static void to_json(json &j, const PlayCameraPathObjective &obj) {
			auto &worldObjective = static_cast<const WorldObjective&>(obj);
			adl_serializer<WorldObjective>::to_json(j, worldObjective);
			auto &a = const_cast<PlayCameraPathObjective&>(obj);
			j["cameraPathUuid"] = a.getCameraPathUuid();
			j["text"] = a.getText();
			j["textTime"] = a.getTextTime();
			j["async"] = a.isAsync();
			j["startFadeOut"] = a.isStartFadeOut();
			j["startFadeOutTime"] = a.getStartFadeOutTime();
			j["endFadeIn"] = a.isEndFadeIn();
			j["endFadeInTime"] = a.getEndFadeInTime();
			j["lockPlayerControl"] = a.isLockPlayerControl();
		}

		static void from_json(const json &j, PlayCameraPathObjective &obj) {
			auto &worldObjective = static_cast<WorldObjective&>(obj);
			j.get_to(worldObjective);
			j.at("cameraPathUuid").get_to(obj.getCameraPathUuid());
			j.at("text").get_to(obj.getText());
			j.at("textTime").get_to(obj.getTextTime());
			j.at("async").get_to(obj.isAsync());
			j.at("startFadeOut").get_to(obj.isStartFadeOut());
			j.at("startFadeOutTime").get_to(obj.getStartFadeOutTime());
			j.at("endFadeIn").get_to(obj.isEndFadeIn());
			j.at("endFadeInTime").get_to(obj.getEndFadeInTime());
			j.at("lockPlayerControl").get_to(obj.isLockPlayerControl());
		}
	};

NLOHMANN_JSON_NAMESPACE_END
