#include <sol.hpp>

#include "utilsRender.h"

void imguiCustomWrapper(sol::state &state) {
	auto imguiTable = state["ImGui"].get_or_create<sol::table>();
	imguiTable.set("IncorrectHighlight", IncorrectHighlight);
}
