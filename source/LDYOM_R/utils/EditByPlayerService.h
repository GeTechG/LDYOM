﻿#pragma once
#include <CObject.h>


class Vehicle;
class Actor;

class EditByPlayerService {
private:
	EditByPlayerService() = default;
	EditByPlayerService(const EditByPlayerService& root) = delete;
	EditByPlayerService& operator=(const EditByPlayerService&) = delete;
public:
	static EditByPlayerService& getInstance() {
		static EditByPlayerService instance;
		return instance;
	}

	void editByPlayerActor(Actor& actor);
	void editByPlayerVehicle(Vehicle& vehicle);
	void positionalObject(CEntity* entity, std::function<void(CMatrix&)> setMatrix, float* posO, CQuaternion& quat, bool fastCreate = true);
};


