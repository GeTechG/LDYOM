#include <sol.hpp>

#include "constants.h"
#include "../Data/WaitSignalObjective.h"

void otherWrapper(sol::state &state) {
	state.set("LDYOM_VERSION", LDYOM_VERSION);
	state["toVoidPtr"] = [](const sol::object &obj) -> const void* {
		return obj.pointer();
	};
	state.set("emitSignal", &WaitSignalObjective::emitSignal);
}
