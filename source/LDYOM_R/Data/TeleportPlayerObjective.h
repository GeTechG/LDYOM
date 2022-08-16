#pragma once
#include <CPed.h>

#include "PlayerObjective.h"

class CVector;

class TeleportPlayerObjective final: public virtual PlayerObjective {
private:
	friend class boost::serialization::access;

	template <class Archive>
	void serialize(Archive& ar, const unsigned version) {
		ar & boost::serialization::base_object<PlayerObjective>(*this);
		ar & boost::serialization::make_array(pos_.data(), pos_.size());
		ar & headingAngle_;
		ar & modelType_;
		ar & slot_;
		ar & modelId_;
		ar & health_;
		ar & interiorID_;
		ar & boost::serialization::make_array(clotherMAnModelKeys_.data(), clotherMAnModelKeys_.size());
		ar & boost::serialization::make_array(clotherMAnTextureKeys_.data(), clotherMAnTextureKeys_.size());
		ar & fatStat;
		ar & musculeStat;
	}

	std::array<float, 3> pos_{};
	float headingAngle_{};
	unsigned char modelType_{};
	int slot_{};
	int modelId_{};
	int health_ = 100;
	int interiorID_ = 0;
	std::array<unsigned, 10> clotherMAnModelKeys_{};
	std::array<unsigned, 18> clotherMAnTextureKeys_{};
	float fatStat = 1.f;
	float musculeStat = 1.f;

	std::optional<CPed*> editorPed_;

	CPed* spawnPed();
	void characteristicsSection(Localization& local);
public:
	TeleportPlayerObjective() = default;
	explicit TeleportPlayerObjective(CVector& pos, float heading);
	~TeleportPlayerObjective() override;

	int getTypeCategory() override {
		return 0;
	}

	void draw(Localization& local) override;
	ktwait execute(Scene* scene, Result& result, ktcoro_tasklist& tasklist) override;

	void spawnEditorPed();
	void deleteEditorPed();
};
