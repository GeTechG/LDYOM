#include "DataLuaWrapper.h"

extern void ProjectDataLuaWrap(sol::state& state);
extern void SceneLuaWrap(sol::state& state);
extern void BaseObjectiveLuaWrap(sol::state& state);
extern void ActorLuaWrap(sol::state& state);
extern void AudioLuaWrap(sol::state& state);
extern void CheckpointLuaWrap(sol::state& state);
extern void ObjectLuaWrap(sol::state& state);
extern void ParticleLuaWrap(sol::state& state);
extern void PickupLuaWrap(sol::state& state);
extern void PyrotechnicsLuaWrap(sol::state& state);
extern void TrainLuaWrap(sol::state& state);
extern void VehicleLuaWrap(sol::state& state);
extern void VisualEffectLuaWrap(sol::state& state);

void DataLuaWrapper::wrap(sol::state& state) {
	ProjectDataLuaWrap(state);
	SceneLuaWrap(state);
	BaseObjectiveLuaWrap(state);
	ActorLuaWrap(state);
	AudioLuaWrap(state);
	CheckpointLuaWrap(state);
	ObjectLuaWrap(state);
	ParticleLuaWrap(state);
	PickupLuaWrap(state);
	PyrotechnicsLuaWrap(state);
	TrainLuaWrap(state);
	VehicleLuaWrap(state);
	VisualEffectLuaWrap(state);
}
