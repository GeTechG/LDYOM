#pragma once
#include "SceneObjective.h"
#include "../Windows/PopupSpriteBlipSelector.h"

class GoToSceneObjective final: public virtual SceneObjective {
private:
	friend class boost::serialization::access;

	template <class Archive>
	void serialize(Archive& ar, const unsigned version) {
		ar & boost::serialization::base_object<SceneObjective>(*this);
		ar & type_;
		ar & triggerUuid_;
		ar & fade_;
		ar & fadeInTime_;
		ar & fadeOutTime_;
		ar & blipColor_;
		ar & blipType_;
		ar & blipSprite_;
		ar & sceneId_;
		ar & startObjective_;
	}

	int type_ = 0;
	boost::uuids::uuid triggerUuid_{};
	bool fade_ = false;
	float fadeInTime_ = 1.f;
	float fadeOutTime_ = 1.f;
	int   blipColor_ = 0;
	int blipType_ = 0;
	int blipSprite_ = 0;
	int sceneId_ = NULL;
	int startObjective_ = 0;

	std::optional<int> editorBlip;
	std::optional<int> projectBlip;

	PopupSpriteBlipSelector popupSpriteBlipSelector_;
public:
	GoToSceneObjective() = default;
	explicit GoToSceneObjective(void* _new);
	~GoToSceneObjective() override = default;

	int getTypeCategory() override {
		return 0;
	}

	void draw(Localization &local, std::vector<std::string> &listOverlay) override;
	ktwait execute(Scene * scene, Result & result, ktcoro_tasklist & tasklist) override;

	void spawnEditorBlip();
	void deleteEditorBlip();

	void deleteProjectBlip();

	bool& isFade();
	float& getFadeInTime();
	float& getFadeOutTime();
	int& getSceneId();
	int& getStartObjective();
};
