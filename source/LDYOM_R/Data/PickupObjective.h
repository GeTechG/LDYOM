#pragma once

#include <CPickup.h>

#include "BaseObjective.h"

class Pickup;

class PickupObjective : virtual public BaseObjective {
private:
	friend class boost::serialization::access;

	template <class Archive>
	void serialize(Archive& ar, const unsigned version) {
		ar& boost::serialization::base_object<BaseObjective>(*this);
		ar& pickupUuid_;
		ar& boost::serialization::make_array(text_.data(), text_.size());
		ar& textTime_;
		ar& colorBlip_;
	}

	boost::uuids::uuid pickupUuid_{};
	std::array<char, TEXT_SIZE> text_{ "" };
	float textTime_ = 1.f;
	int colorBlip_ = 0;

	std::array<char, TEXT_SIZE> gameText_ = { "" };
protected:
	PickupObjective() = default;

	std::optional<int> editorBlip_;
	std::optional<int> projectBlip_;

	int spawnBlip(int pickup);

	void spawnEditorBlip();
	void removeEditorBlip();

	void removeProjectBlip();

	boost::uuids::uuid& getPickupUuid();
	std::array<char, TEXT_SIZE>& getText();
	float& getTextTime();
	int& getColorBlip();
	std::optional<int>& getEditorBlip();
	std::optional<int>& getProjectBlip();

	virtual ktwait execute(Scene* scene, Pickup* pickup, Result& result, ktcoro_tasklist& tasklist) = 0;
public:
	~PickupObjective() override;

	int getCategory() override {
		return 4;
	}

	void draw(Localization& local) override;
	void open() override;
	void close() override;
	ktwait execute(Scene* scene, Result& result, ktcoro_tasklist& tasklist) override;
};
