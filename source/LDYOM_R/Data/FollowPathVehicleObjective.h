#pragma once
#include "VehicleObjective.h"

class FollowPathVehicleObjective final: public virtual VehicleObjective {
private:
	friend class boost::serialization::access;

	template <class Archive>
	void serialize(Archive& ar, const unsigned version) {
		ar & boost::serialization::base_object<VehicleObjective>(*this);
		ar & driveType_;
		ar & pathType_;
		ar & executeTime_;
		ar & path_;
		ar & waitEnd_;
	}

	int driveType_ = 0;
	int pathType_ = 0;
	float executeTime_ = 1.f;
	std::vector<std::array<float, 3>> path_;
	bool waitEnd_ = false;
public:
	FollowPathVehicleObjective() = default;
	explicit FollowPathVehicleObjective(void* _new);
	~FollowPathVehicleObjective() override = default;

	int getTypeCategory() override {
		return 2;
	}

	void draw(Localization& local) override;
	ktwait execute(Scene* scene, Vehicle* vehicle, Result& result, ktcoro_tasklist& tasklist) override;

	int& getDriveType();
	int& getPathType();
	float& getExecuteTime();
	std::vector<std::array<float, 3>>& getPath();
};
