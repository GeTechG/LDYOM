#pragma once
#include "PlayerObjective.h"

class TeleportToVehiclePlayerObjective final: public virtual PlayerObjective {
private:
	friend class boost::serialization::access;

	template <class Archive>
	void serialize(Archive& ar, const unsigned version) {
		ar & boost::serialization::base_object<PlayerObjective>(*this);
		ar & vehicleUuid_;
		ar & place_;
	}

	boost::uuids::uuid vehicleUuid_;
	int place_ = 0;
public:
	TeleportToVehiclePlayerObjective() = default;
	explicit TeleportToVehiclePlayerObjective(void* _new);
	~TeleportToVehiclePlayerObjective() override = default;

	int getTypeCategory() override {
		return 2;
	}

	void draw(Localization & local) override;
	ktwait execute(Scene * scene, Result & result, ktcoro_tasklist & tasklist) override;
};
