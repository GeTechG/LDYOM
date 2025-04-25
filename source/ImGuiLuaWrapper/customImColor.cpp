#include <cimgui.h>
#include <sol.hpp>

void bindImColor(sol::state &state) {
	auto classImColor =
		state.new_usertype<ImColor>("ImColor", sol::factories(
			                            []() {
				                            auto result = ImColor_ImColor_Nil();
				                            return std::shared_ptr<ImColor>(result, ImColor_destroy);
			                            },
			                            [](const float r, const float g, const float b, const float a) {
				                            if (r > 1. && g > 1. && b > 1. && a > 1.) {
					                            return std::shared_ptr<ImColor>(
						                            ImColor_ImColor_Int(static_cast<int>(r), static_cast<int>(g),
						                                                static_cast<int>(b), static_cast<int>(a)),
						                            ImColor_destroy);
				                            }
				                            return std::shared_ptr<ImColor>(
					                            ImColor_ImColor_Float(r, g, b, a), ImColor_destroy);
			                            },
			                            [](const ImVec4 col) {
				                            auto result = ImColor_ImColor_Vec4(col);
				                            return std::shared_ptr<ImColor>(result, ImColor_destroy);
			                            },
			                            [](const ImU32 rgba) {
				                            auto result = ImColor_ImColor_U32(rgba);
				                            return std::shared_ptr<ImColor>(result, ImColor_destroy);
			                            }));
	classImColor["Value"] = &ImColor::Value;
	classImColor["HSV"] = &ImColor_HSV;
	classImColor["SetHSV"] = &ImColor_SetHSV;
}
