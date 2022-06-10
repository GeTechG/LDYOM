#pragma once

#include "IPositionable.h"
#include "IRenderable.h"
#include "WorldObjective.h"
#include "boost/serialization/array.hpp"
#include "Localization/Localization.h"

#include "../Windows/PopupSpriteBlipSelector.h"

class CheckpointObjective final : virtual public WorldObjective, public IRenderable, public IPositionable {
private:

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<WorldObjective>(*this);
		ar & this->pos_;
		ar & this->text_;
		ar & this->onWhatArrive_;
		ar & this->radius_;
		ar & this->textTime_;
		ar & this->blipColor_;
		ar & this->indexSphere_;
		ar & this->comeBackVehicle_;
		ar & this->textComeBackVehicle_;
		ar & this->colorBlipComeBackVehicle_;
		ar & this->sphereColor_;
		ar & boost::serialization::make_array(this->sphereColor_.data(), this->sphereColor_.size());
		ar & this->pulsePeriod;
		ar & this->pulseFraction;
		ar & this->rotateRate;
	}

	float pos_[3]{};
	char  text_[TEXT_SIZE]{};
	int   onWhatArrive_;
	float radius_;
	float textTime_;
	int   blipColor_;
	int   indexSphere_;
	boost::uuids::uuid  comeBackVehicle_;
	char  textComeBackVehicle_[TEXT_SIZE]{};
	int   colorBlipComeBackVehicle_;
	std::array<float, 4> sphereColor_;
	int sphereType;
	int pulsePeriod;
	float pulseFraction;
	int rotateRate;
	int blipType_;
	int blipSprite_;
	std::optional<int> editorBlip;
	std::optional<int> projectBlip;
	std::optional<int> projectComeBackBlip;

	char gameText[TEXT_SIZE] = "";
	bool rerender = false;
	PopupSpriteBlipSelector popupSpriteBlipSelector_;
public:
	CheckpointObjective();
	explicit CheckpointObjective(const float* pos);

	CheckpointObjective(const CheckpointObjective& other);

	CheckpointObjective& operator=(CheckpointObjective other);

	~CheckpointObjective() override;

	int getTypeCategory() override {
		return 0;
	}

	void render() override;
	ktwait execute(Scene* scene) override;
	void draw(Localization& local) override;

	char* getText();
	char* getTextComeBackVehicle();
	float& getRadius();
	float& getTextTime();
	int& getColorBlip();
	int& getIndexSphere();
	int& getOnWhatArrive();
	boost::uuids::uuid& getComeBackVehicle();
	int& getColorBlipComeBackVehicle();
	std::array<float, 4>& getSphereColor();
	int& getSphereType();
	int& getPulsePeriod();
	float& getPulseFraction();
	int& getRotateRate();
	int& getBlipType();
	int& getSpriteBlip();
	std::optional<int>& getEditorBlip();
	std::optional<int>& getProjectBlip();
	std::optional<int>& getProjectComeBackBlip();

	void createEditorBlip();
	void createProjectBlip();

	void removeEditorBlip();
	void removeProjectBlip();
	void removeProjectComeBackBlip();

	bool& isRerender();

	float* getPosition() override;
};
