#include <sol.hpp>

#include "constants.h"

void otherWrapper(sol::state &state) {
	state.set("LDYOM_VERSION", LDYOM_VERSION);
}
