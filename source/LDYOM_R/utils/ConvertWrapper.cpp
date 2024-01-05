#include <CPools.h>
#include <sol.hpp>

void convertWrapper(sol::state &state) {
	state.set("CPedToHandle", [](CPed *ped) {
		return CPools::GetPedRef(ped);
	});
	state.set("CVehicleToHandle", [](CVehicle *vehicle) {
		return CPools::GetVehicleRef(vehicle);
	});
	state.set("CObjectToHandle", [](CObject *object) {
		return CPools::GetObjectRef(object);
	});
}
