#pragma once
#include "ActorObjective.h"

class DamageActorObjective final : virtual public ActorObjective {
private:
	friend class boost::serialization::access;

	template <class Archive>
	void serialize(Archive& ar, const unsigned version) {
		ar & boost::serialization::base_object<ActorObjective>(*this);
	}

public:
	DamageActorObjective() = default;
	explicit DamageActorObjective(void* _new);
	~DamageActorObjective() override = default;

	int getTypeCategory() override {
		return 1;
	}

	ktwait execute(Scene* scene, Actor* actor, Result& result, ktcoro_tasklist& tasklist) override;
};
