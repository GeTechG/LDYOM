#pragma once
#include <CVehicle.h>

#include "BaseObjective.h"

class Vehicle;

class VehicleObjective : virtual public BaseObjective {
protected:
	friend class boost::serialization::access;

	template <class Archive>
	void serialize(Archive &ar, const unsigned version) {
		ar & boost::serialization::base_object<BaseObjective>(*this);
		ar & vehicleUuid_;
		ar & boost::serialization::make_array(text_.data(), text_.size());
		ar & textTime_;
		ar & colorBlip_;
	}

	boost::uuids::uuid vehicleUuid_{};
	std::array<char, TEXT_SIZE> text_{""};
	float textTime_ = 1.f;
	int colorBlip_ = 0;

	std::array<char, TEXT_SIZE> gameText_ = {""};

	VehicleObjective() = default;

	std::optional<int> editorBlip_;
	std::optional<int> projectBlip_;

	int spawnBlip(CVehicle *vehicle);

	void spawnEditorBlip();
	void removeEditorBlip();

	void removeProjectBlip();

	boost::uuids::uuid& getVehicleUuid();
	std::array<char, TEXT_SIZE>& getText();
	float& getTextTime();
	int& getColorBlip();
	std::optional<int>& getEditorBlip();
	std::optional<int>& getProjectBlip();

	virtual ktwait execute(Scene *scene, Vehicle *vehicle, Result &result, ktcoro_tasklist &tasklist) = 0;

public:
	~VehicleObjective() override;

	int getCategory() override {
		return 2;
	}

	void draw(Localization &local, std::vector<std::string> &listOverlay) override;
	void open() override;
	void close() override;
	ktwait execute(Scene *scene, Result &result, ktcoro_tasklist &tasklist) override;
};
