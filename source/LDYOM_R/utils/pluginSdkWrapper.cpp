#include <CSprite.h>
#include <CTimer.h>
#include <plugin.h>
#include <sol.hpp>
#include <extensions/KeyCheck.h>

void pluginSdkWrapper(sol::state &state) {
	state["time"].get_or_create<sol::table>().set("snTimeInMilliseconds", &CTimer::m_snTimeInMilliseconds);
	state.set("distanceBetweenPoints",
	          [](const float x1, const float y1, const float z1, const float x2, const float y2, const float z2) {
		          return DistanceBetweenPoints(CVector(x1, y1, z1), CVector(x2, y2, z2));
	          });
	state.set("GetPedRef", CPools::GetPedRef);
	state.set("GetVehicleRef", [](sol::userdata ptr) {
		const auto vehicle = *static_cast<CVehicle**>(const_cast<void*>(ptr.pointer()));
		return CPools::GetVehicleRef(vehicle);
	});
	state.set("GetObjectRef", CPools::GetObjectRef);
	state.set("CalcScreenCoors",
	          [](const sol::table &posn, sol::table out, bool checkMaxVisible, bool checkMinVisible) {
		          RwV3d outRw{};
		          float w, h;
		          auto result = CSprite::CalcScreenCoors(RwV3d{posn[1], posn[2], posn[3]}, &outRw, &w, &h,
		                                                 checkMaxVisible, checkMinVisible);
		          out[1] = outRw.x;
		          out[2] = outRw.y;
		          return std::make_tuple(result, w, h);
	          });
	auto keyCheckTable = state.create_table("KeyCheck");
	keyCheckTable.set("Update", KeyCheck::Update);
	keyCheckTable.set("CheckJustDown", KeyCheck::CheckJustDown);
}
