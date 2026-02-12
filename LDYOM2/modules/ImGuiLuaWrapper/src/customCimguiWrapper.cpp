#include <cimgui.h>
#include <sol/sol.hpp>

void bindCustomCimGui(sol::state &state) {
	auto classImGui = state["ImGui"].get_or_create<sol::table>();
	classImGui["CalcTextSize"] = [](const char *text, const sol::object &endText,
	                                const bool hideTextAfterDoubleHash,
	                                const float wrapWidth) -> ImVec2 {
		return igCalcTextSize(text, endText.get_type() == sol::type::lua_nil ? nullptr : endText.as<const char*>(),
		                      hideTextAfterDoubleHash, wrapWidth);
	};
}
