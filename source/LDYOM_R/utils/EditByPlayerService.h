#pragma once
#include <CObject.h>
#include <sol.hpp>

#include "CarrecPathsService.h"


class Train;
class CutsceneObjective;
class Vehicle;
class Actor;

using CallbackCarrecPath = std::function<void(bool, std::vector<CVehicleStateEachFrame>,
                                              const std::map<int, std::vector<CVehicleStateEachFrame>> &)>;

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
	void editByPlayerCarrecPath(
		int type, CarrecPath &playerPath,
		const std::vector<CarrecPath*> &previewPaths, const std::vector<bool> &useAIRerecord,
		CallbackCarrecPath callback);
};
