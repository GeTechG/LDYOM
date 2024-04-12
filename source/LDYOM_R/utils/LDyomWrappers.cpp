#include <sol.hpp>

extern void projectsServiceWrapper(sol::state &state);
extern void projectInfoWrapper(sol::state &state);
extern void projectDataWrapper(sol::state &state);
extern void sceneWrapper(sol::state &state);
extern void objectiveWrapper(sol::state &state);
extern void actorWrapper(sol::state &state);
extern void vehicleWrapper(sol::state &state);
extern void objectWrapper(sol::state &state);
extern void particleWrapper(sol::state &state);
extern void trainWrapper(sol::state &state);
extern void pickupWrapper(sol::state &state);
extern void pyrotechnicsWrapper(sol::state &state);
extern void audioWrapper(sol::state &state);
extern void visualEffectWrapper(sol::state &state);
extern void checkpointWrapper(sol::state &state);
extern void sceneSettingsWrapper(sol::state &state);
extern void weaponWrapper(sol::state &state);
extern void cameraPathWrapper(sol::state &state);

extern void eventsWrapper(sol::state &state);
extern void settingsWrapper(sol::state &state);
extern void taskerWrapper(sol::state &state);
extern void hotKeyServiceWrapper(sol::state &state);
extern void modelsServiceWrapper(sol::state &state);
extern void carrecPathServiceWrapper(sol::state &state);
extern void cameraExtendWrapper(sol::state &state);


void ldyomWrapper(sol::state &state) {
	projectsServiceWrapper(state);
	projectInfoWrapper(state);
	projectDataWrapper(state);
	sceneWrapper(state);
	objectiveWrapper(state);
	actorWrapper(state);
	vehicleWrapper(state);
	objectWrapper(state);
	particleWrapper(state);
	trainWrapper(state);
	pickupWrapper(state);
	pyrotechnicsWrapper(state);
	audioWrapper(state);
	visualEffectWrapper(state);
	checkpointWrapper(state);
	sceneSettingsWrapper(state);
	weaponWrapper(state);
	cameraPathWrapper(state);
	eventsWrapper(state);
	settingsWrapper(state);
	taskerWrapper(state);
	hotKeyServiceWrapper(state);
	modelsServiceWrapper(state);
	carrecPathServiceWrapper(state);
	cameraExtendWrapper(state);
}
