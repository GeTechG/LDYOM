#pragma once
#include "ActorObjective.h"

class AnimationActorObjective final: public virtual ActorObjective {
private:
	friend class boost::serialization::access;

	template <class Archive>
	void serialize(Archive& ar, const unsigned version) {
		ar & boost::serialization::base_object<ActorObjective>(*this);
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
	AnimationActorObjective() = default;
	explicit AnimationActorObjective(void* _new);
	~AnimationActorObjective() override = default;

	int getTypeCategory() override {
		return 3;
	}

	void draw(Localization & local) override;
	ktwait execute(Scene * scene, Actor * actor, Result & result, ktcoro_tasklist & tasklist) override;

	void close() override;
};
