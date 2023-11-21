#pragma once
#include "SceneObjective.h"
#include "../Windows/PopupSpriteBlipSelector.h"

class GoToSceneObjective final : public virtual SceneObjective {
private:
	int type_ = 0;
	boost::uuids::uuid triggerUuid_{};
	bool fade_ = false;
	float fadeInTime_ = 1.f;
	float fadeOutTime_ = 1.f;
	int blipColor_ = 0;
	int blipType_ = 0;
	int blipSprite_ = 0;
	int sceneId_ = NULL;
	int startObjective_ = 0;

	std::optional<int> editorBlip;
	std::optional<int> projectBlip;

	PopupSpriteBlipSelector popupSpriteBlipSelector_;

public:
	GoToSceneObjective() = default;
	explicit GoToSceneObjective(void *_new);
	~GoToSceneObjective() override = default;

	int getTypeCategory() override {
		return 0;
	}

	void draw(Localization &local, std::vector<std::string> &listOverlay) override;
	ktwait execute(Scene *scene, Result &result, ktcoro_tasklist &tasklist) override;

	void spawnEditorBlip();
	void deleteEditorBlip();

	void deleteProjectBlip();

	bool& isFade();
	float& getFadeInTime();
	float& getFadeOutTime();
	int& getSceneId();
	int& getStartObjective();
	int& getType();
	boost::uuids::uuid& getTriggerUuid();
	int& getBlipColor();
	int& getBlipType();
	int& getBlipSprite();
};

NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<GoToSceneObjective> {
		static void to_json(json &j, const GoToSceneObjective &obj) {
			auto &sceneObjective = static_cast<const SceneObjective&>(obj);
			adl_serializer<SceneObjective>::to_json(j, sceneObjective);

			auto &a = const_cast<GoToSceneObjective&>(obj);
			j["type"] = a.getType();
			j["triggerUuid"] = a.getTriggerUuid();
			j["fade"] = a.isFade();
			j["fadeInTime"] = a.getFadeInTime();
			j["fadeOutTime"] = a.getFadeOutTime();
			j["blipColor"] = a.getBlipColor();
			j["blipType"] = a.getBlipType();
			j["blipSprite"] = a.getBlipSprite();
			j["sceneId"] = a.getSceneId();
			j["startObjective"] = a.getStartObjective();
		}

		static void from_json(const json &j, GoToSceneObjective &obj) {
			auto &sceneObjective = static_cast<SceneObjective&>(obj);
			j.get_to(sceneObjective);

			j.at("type").get_to(obj.getType());
			j.at("triggerUuid").get_to(obj.getTriggerUuid());
			j.at("fade").get_to(obj.isFade());
			j.at("fadeInTime").get_to(obj.getFadeInTime());
			j.at("fadeOutTime").get_to(obj.getFadeOutTime());
			j.at("blipColor").get_to(obj.getBlipColor());
			j.at("blipType").get_to(obj.getBlipType());
			j.at("blipSprite").get_to(obj.getBlipSprite());
			j.at("sceneId").get_to(obj.getSceneId());
			j.at("startObjective").get_to(obj.getStartObjective());
		}
	};

NLOHMANN_JSON_NAMESPACE_END
