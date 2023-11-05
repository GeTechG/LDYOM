#pragma once

#include "WorldObjective.h"
#include "boost/serialization/array.hpp"
#include "Localization/Localization.h"

#include "../Windows/PopupSpriteBlipSelector.h"

class CheckpointObjective final : virtual public WorldObjective {
private:
	friend class boost::serialization::access;

	template <class Archive>
	void serialize(Archive &ar, const unsigned int version) {
		ar & boost::serialization::base_object<WorldObjective>(*this);
		ar & this->checkpointUuid_;
		ar & this->text_;
		ar & this->onWhatArrive_;
		ar & this->textTime_;
		ar & this->comeBackVehicle_;
		ar & this->textComeBackVehicle_;
		ar & this->colorBlipComeBackVehicle_;
	}

	boost::uuids::uuid checkpointUuid_;
	std::array<char, TEXT_SIZE> text_;
	int onWhatArrive_;
	float textTime_;
	boost::uuids::uuid comeBackVehicle_;
	std::array<char, TEXT_SIZE> textComeBackVehicle_;
	int colorBlipComeBackVehicle_;
	std::optional<int> projectComeBackBlip_;

	std::array<char, TEXT_SIZE> gameText_ = {""};
	PopupSpriteBlipSelector popupSpriteBlipSelector_;

public:
	CheckpointObjective() = default;
	explicit CheckpointObjective(void *_new);

	CheckpointObjective(const CheckpointObjective &other);
	CheckpointObjective& operator=(const CheckpointObjective &other);
	~CheckpointObjective() override;

	int getTypeCategory() override {
		return 0;
	}

	ktwait execute(Scene *scene, Result &result, ktcoro_tasklist &tasklist) override;
	void draw(Localization &local, std::vector<std::string> &listOverlay) override;

	boost::uuids::uuid& getCheckpointUuid();
	std::array<char, TEXT_SIZE>& getText();
	int& getOnWhatArrive();
	float& getTextTime();
	boost::uuids::uuid& getComeBackVehicle();
	std::array<char, TEXT_SIZE>& getTextComeBackVehicle();
	int& getColorBlipComeBackVehicle();
	std::optional<int>& getProjectComeBackBlip();
	std::array<char, TEXT_SIZE>& getGameText();

	void removeProjectComeBackBlip();
};
