#pragma once
#include <CObject.h>
#include <sol.hpp>


class Train;
class CutsceneObjective;
class Vehicle;
class Actor;

class EditByPlayerService {
private:
	EditByPlayerService() = default;
	EditByPlayerService(const EditByPlayerService &root) = delete;
	EditByPlayerService& operator=(const EditByPlayerService &) = delete;

public:
	static EditByPlayerService& getInstance() {
		static EditByPlayerService instance;
		return instance;
	}

	void editByPlayerActor(Actor &actor);
	void editByPlayerVehicle(Vehicle &vehicle);
	void positionalObject(CEntity *entity, std::function<void(CMatrix &)> setMatrix, float *posO, CQuaternion &quat,
	                      bool fastCreate = true);
	void editByPlayerCamera(float *pos, CQuaternion *rotation, bool widescreen, std::function<void()> callback);
	void editByPlayerActorPath(std::vector<std::array<float, 3>> &path);
	void editByPlayerVehiclePath(std::vector<std::array<float, 3>> &path, int model);
	void editByPlayerTrain(Train &train);
};
