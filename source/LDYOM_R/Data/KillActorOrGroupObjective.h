#pragma once
#include "ActorObjective.h"

class KillActorOrGroupObjective final : virtual public ActorObjective {
private:
	friend class boost::serialization::access;

	template <class Archive>
	void serialize(Archive& ar, const unsigned version) {
		ar & boost::serialization::base_object<ActorObjective>(*this);
		ar & killGroup_;
	}

	bool killGroup_ = false;
public:
	KillActorOrGroupObjective() = default;
	explicit KillActorOrGroupObjective(void* _new);
	~KillActorOrGroupObjective() override = default;

	int getTypeCategory() override {
		return 0;
	}

	void draw(Localization& local) override;
	ktwait execute(Scene* scene, Actor* actor, Result& result, ktcoro_tasklist& tasklist) override;

	bool& isKillGroup();
};
