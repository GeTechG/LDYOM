#include <sol.hpp>
#include "../Data/Scene.h"

void sceneWrapper(sol::state &state) {
	state.new_usertype<Scene>("LDScene",
	                          sol::no_constructor,
	                          "getName", &Scene::getName,
	                          "getId", &Scene::getId,
	                          "getObjectives", &Scene::getObjectives,
	                          "getActors", &Scene::getActors,
	                          "getVehicles", &Scene::getVehicles,
	                          "getObjects", &Scene::getObjects,
	                          "getParticles", &Scene::getParticles,
	                          "getTrains", &Scene::getTrains,
	                          "getPickups", &Scene::getPickups,
	                          "getPyrotechnics", &Scene::getPyrotechnics,
	                          "getAudio", &Scene::getAudio,
	                          "getVisualEffects", &Scene::getVisualEffects,
	                          "getCheckpoints", &Scene::getCheckpoints,
	                          "getSceneSettings", &Scene::getSceneSettings,
	                          "isToggleSceneSettings", &Scene::isToggleSceneSettings,
	                          "createNewActor", &Scene::createNewActor,
	                          "createNewVehicle", &Scene::createNewVehicle,
	                          "createNewObject", &Scene::createNewObject,
	                          "createNewParticle", &Scene::createNewParticle,
	                          "createNewTrain", &Scene::createNewTrain,
	                          "createNewPickup", &Scene::createNewPickup,
	                          "createNewPyrotechnics", &Scene::createNewPyrotechnics,
	                          "createNewAudio", &Scene::createNewAudio,
	                          "createNewVisualEffect", &Scene::createNewVisualEffect,
	                          "createNewCheckpoint", &Scene::createNewCheckpoint,
	                          "updateEditorObjectsCollision", &Scene::updateEditorObjectsCollision,
	                          "unloadEditorScene", &Scene::unloadEditorScene,
	                          "unloadProjectScene", &Scene::unloadProjectScene,
	                          "loadEditorScene", &Scene::loadEditorScene
	);
}
