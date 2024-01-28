#include <cimgui.h>
#include <sol.hpp>

void bindCustomCimGui(sol::state &state) {
	auto classImGui = state["ImGui"].get_or_create<sol::table>();
	classImGui["CalcTextSize"] = [](ImVec2 *pOut, const char *text, const sol::object &endText,
	                                const bool hideTextAfterDoubleHash,
	                                const float wrapWidth) {
		igCalcTextSize(pOut, text, endText.get_type() == sol::type::nil ? nullptr : endText.as<const char*>(),
		               hideTextAfterDoubleHash, wrapWidth);
	};
}
