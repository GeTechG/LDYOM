#pragma once
#include <array>
#include <nlohmann/json.hpp>
#include "jsonUtils.h"


struct SceneSettings {
	std::array<std::array<int, 9>, 8> groupRelations{};
	std::array<signed char, 2> time = {0, 0};
	float trafficPed = 1.f;
	float trafficCar = 1.f;
	int wantedMin = 0;
	int wantedMax = 6;
	int weather = 0;
	bool riot = false;
};

NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<SceneSettings> {
		static void to_json(json &j, const SceneSettings &w) {
			j = json{
				{"groupRelations", w.groupRelations},
				{"time", w.time},
				{"trafficPed", w.trafficPed},
				{"trafficCar", w.trafficCar},
				{"wantedMin", w.wantedMin},
				{"wantedMax", w.wantedMax},
				{"weather", w.weather},
				{"riot", w.riot}
			};
		}

		static void from_json(const json &j, SceneSettings &w) {
			j.at("groupRelations").get_to(w.groupRelations);
			j.at("time").get_to(w.time);
			j.at("trafficPed").get_to(w.trafficPed);
			j.at("trafficCar").get_to(w.trafficCar);
			j.at("wantedMin").get_to(w.wantedMin);
			j.at("wantedMax").get_to(w.wantedMax);
			j.at("weather").get_to(w.weather);
			j.at("riot").get_to(w.riot);
		}
	};

NLOHMANN_JSON_NAMESPACE_END
