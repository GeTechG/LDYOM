#include <sol.hpp>

#include "imgui.h"
#include "imgui_stdlib.h"

void bindCustomImGui(sol::state &state) {
	auto classImGui = state["ImGui"].get_or_create<sol::table>();
	classImGui["TextUnformatted"] = [](const char *text, const sol::object &endText) {
		ImGui::TextUnformatted(text, endText.get_type() == sol::type::nil ? nullptr : endText.as<const char*>());
	};
	classImGui["InputText"] = [
		](const char *label, std::string str, const int flags, const sol::object &callback, void *userData) {
			bool result = ImGui::InputText(label, &str, flags,
			                               callback.get_type() == sol::type::nil
				                               ? nullptr
				                               : callback.as<ImGuiInputTextCallback>(), userData);
			return std::make_tuple(result, str);
		};
}
