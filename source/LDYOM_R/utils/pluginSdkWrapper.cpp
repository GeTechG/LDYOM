#include <CTimer.h>
#include <plugin.h>
#include <sol.hpp>

void pluginSdkWrapper(sol::state &state) {
	state["time"].get_or_create<sol::table>().set("snTimeInMilliseconds", &CTimer::m_snTimeInMilliseconds);
	state.set("distanceBetweenPoints",
	          [](const float x1, const float y1, const float z1, const float x2, const float y2, const float z2) {
		          return DistanceBetweenPoints(CVector(x1, y1, z1), CVector(x2, y2, z2));
	          });
}
