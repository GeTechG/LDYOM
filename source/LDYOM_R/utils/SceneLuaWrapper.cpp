#include <sol.hpp>
#include "../Data/Scene.h"

void SceneLuaWrap(sol::state& state) {
	auto clazz = state.new_usertype<Scene>("Scene");
	clazz["getName"] = &Scene::getName;
	clazz["getId"] = &Scene::getId;
	clazz["getObjectives"] = &Scene::getObjectives;
	clazz["getActors"] = &Scene::getActors;
	clazz["getVehicles"] = &Scene::getVehicles;
	clazz["getObjects"] = &Scene::getObjects;
	clazz["getParticles"] = &Scene::getParticles;
	clazz["getTrains"] = &Scene::getTrains;
	clazz["getPickups"] = &Scene::getPickups;
	clazz["getPyrotechnics"] = &Scene::getPyrotechnics;
	clazz["getAudio"] = &Scene::getAudio;
	clazz["getVisualEffects"] = &Scene::getVisualEffects;
	clazz["getCheckpoints"] = &Scene::getCheckpoints;
	clazz["getSceneSettings"] = &Scene::getSceneSettings;
	clazz["isToggleSceneSettings"] = &Scene::isToggleSceneSettings;
	//clazz["createNewObjectives"] = &Scene::createNewObjectives;
	clazz["createNewActor"] = &Scene::createNewActor;
	clazz["createNewVehicle"] = &Scene::createNewVehicle;
	clazz["createNewObject"] = &Scene::createNewObject;
	clazz["createNewParticle"] = &Scene::createNewParticle;
	clazz["createNewTrain"] = &Scene::createNewTrain;
	clazz["createNewPickup"] = &Scene::createNewPickup;
	clazz["createNewPyrotechnics"] = &Scene::createNewPyrotechnics;
	clazz["createNewAudio"] = &Scene::createNewAudio;
	clazz["createNewVisualEffect"] = &Scene::createNewVisualEffect;
	clazz["createNewCheckpoint"] = &Scene::createNewCheckpoint;
	//clazz["createNewObjectiveFrom"] = &Scene::createNewObjectiveFrom;
	clazz["createNewActorFrom"] = &Scene::createNewActorFrom;
	clazz["createNewVehicleFrom"] = &Scene::createNewVehicleFrom;
	clazz["createNewObjectFrom"] = &Scene::createNewObjectFrom;
	clazz["createNewParticleFrom"] = &Scene::createNewParticleFrom;
	clazz["createNewTrainFrom"] = &Scene::createNewTrainFrom;
	clazz["createNewPickupFrom"] = &Scene::createNewPickupFrom;
	clazz["createNewPyrotechnicsFrom"] = &Scene::createNewPyrotechnicsFrom;
	clazz["createNewAudioFrom"] = &Scene::createNewAudioFrom;
	clazz["createNewVisualEffectFrom"] = &Scene::createNewVisualEffectFrom;
	clazz["createNewCheckpointFrom"] = &Scene::createNewCheckpointFrom;
	clazz["unloadEditorScene"] = &Scene::unloadEditorScene;
	clazz["unloadProjectScene"] = &Scene::unloadProjectScene;
	clazz["loadEditorScene"] = &Scene::loadEditorScene;
}
