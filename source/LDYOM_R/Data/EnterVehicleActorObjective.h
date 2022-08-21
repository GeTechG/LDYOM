#pragma once
#include "ActorObjective.h"

class EnterVehicleActorObjective final: public virtual ActorObjective {
private:
	friend class boost::serialization::access;

	template <class Archive>
	void serialize(Archive& ar, const unsigned version) {
		ar & boost::serialization::base_object<ActorObjective>(*this);
		ar & vehicleUuid_;
		ar & place_;
		ar & teleport_;
		ar & waitExecute_;
	}

	boost::uuids::uuid vehicleUuid_{};
	int place_ = 0;
	bool teleport_ = false;
	bool waitExecute_ = false;
public:
	EnterVehicleActorObjective() = default;
	explicit EnterVehicleActorObjective(void* _new);
	~EnterVehicleActorObjective() override = default;

	int getTypeCategory() override {
		return 4;
	}

	void draw(Localization & local) override;
	ktwait execute(Scene * scene, Actor * actor, Result & result, ktcoro_tasklist & tasklist) override;
};
