#pragma once
#include <array>
#include <ePedType.h>
#include <eRelationshipType.h>
#include <nlohmann/json.hpp>
#include <vector>

struct SceneSettings {
	struct Relation {
		ePedType ofPedType = PED_TYPE_PLAYER1;
		ePedType toPedType = PED_TYPE_PLAYER1;
		eRelationshipType relationType;
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(Relation, ofPedType, toPedType, relationType)
	};
	bool isPrintSceneName = true;
	bool isSceneSettingsEnabled = true;
	bool isShowMissionTime = false;
	bool limitCompletionTime = false;
	int completionTime = 0;
	std::vector<Relation> groupRelations;
	std::array<int, 2> time = {0, 0};
	float trafficPed = 1.f;
	float trafficCar = 1.f;
	int wantedMin = 0;
	int wantedMax = 6;
	int weather = 0;
	bool riot = false;
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(SceneSettings, isPrintSceneName, isSceneSettingsEnabled, groupRelations, time,
	                               trafficPed, trafficCar, wantedMin, wantedMax, weather, riot, isShowMissionTime,
	                               limitCompletionTime, completionTime)
};
