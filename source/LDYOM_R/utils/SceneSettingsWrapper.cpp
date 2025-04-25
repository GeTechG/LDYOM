#include <sol.hpp>
#include "../Data/SceneSettings.h"

void sceneSettingsWrapper(sol::state &state) {
	state.new_usertype<SceneSettings>("SceneSettings",
	                                  sol::no_constructor,
	                                  "groupRelations", &SceneSettings::groupRelations,
	                                  "time", &SceneSettings::time,
	                                  "trafficPed", &SceneSettings::trafficPed,
	                                  "trafficCar", &SceneSettings::trafficCar,
	                                  "wantedMin", &SceneSettings::wantedMin,
	                                  "wantedMax", &SceneSettings::wantedMax,
	                                  "weather", &SceneSettings::weather,
	                                  "riot", &SceneSettings::riot
	);
}
