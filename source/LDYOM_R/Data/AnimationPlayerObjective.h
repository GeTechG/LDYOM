#pragma once
#include "PlayerObjective.h"

class AnimationPlayerObjective final: public virtual PlayerObjective {
private:
	friend class boost::serialization::access;

	template <class Archive>
	void serialize(Archive& ar, const unsigned version) {
		ar & boost::serialization::base_object<PlayerObjective>(*this);
		ar & pack_;
		ar & animation_;
		ar & looped_;
		ar & blend_;
		ar & time_;
	}

	int pack_ = 0;
	int animation_ = 0;
	bool looped_ = false;
	float blend_ = 0.0f;
	float time_ = 0.f;
public:
	AnimationPlayerObjective() = default;
	explicit AnimationPlayerObjective(void* _new);
	~AnimationPlayerObjective() override = default;

	int getTypeCategory() override {
		return 1;
	}

	void draw(Localization & local) override;
	ktwait execute(Scene * scene, Result & result, ktcoro_tasklist & tasklist) override;

	void close() override;
};
