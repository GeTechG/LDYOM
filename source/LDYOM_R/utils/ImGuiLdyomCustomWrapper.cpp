#include <sol.hpp>

#include "imgui.h"
#include "utilsRender.h"

void imguiLdyomCustomWrapper(sol::state &state) {
	auto imguiTable = state["ImGui"].get_or_create<sol::table>();
	imguiTable.set("IncorrectHighlight", IncorrectHighlight);
	imguiTable["GetMouseWheel"] = []() {
		return ImGui::GetIO().MouseWheel;
	};
}
