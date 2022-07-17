#pragma once
#include <array>


struct SceneSettings {
	std::array<std::array<int, 9>, 8> groupRelations{};
	std::array<signed char, 2> time = { 0, 0 };
	float trafficPed = 1.f;
	float trafficCar = 1.f;
	int wantedMin = 0;
	int wantedMax = 6;
	int weather = 0;
	bool riot = false;
};
