#include <sol.hpp>

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_stdlib.h"

void bindCustomImGui(sol::state &state) {
	auto classImGui = state["ImGui"].get_or_create<sol::table>();
	classImGui["TextUnformatted"] = [](const char *text, const sol::object &endText) {
		ImGui::TextUnformatted(text, endText.get_type() == sol::type::nil ? nullptr : endText.as<const char*>());
	};
	classImGui["InputText"] = [
		](const char *label, std::string str, const int flags, const sol::object &callback,
		  const sol::object &userData) {
			bool result = ImGui::InputText(label, &str, flags,
			                               callback.get_type() == sol::type::nil
				                               ? nullptr
				                               : callback.as<ImGuiInputTextCallback>(),
			                               userData.get_type() == sol::type::nil ? nullptr : userData.as<void*>());
			return std::make_tuple(result, str);
		};
	classImGui["InputTextMultiline"] = [
		](const char *label, std::string str, const ImVec2 &size, const int flags, const sol::object &callback,
		  const sol::object &userData) {
			bool result = ImGui::InputTextMultiline(label, &str, size, flags,
			                                        callback.get_type() == sol::type::nil
				                                        ? nullptr
				                                        : callback.as<ImGuiInputTextCallback>(),
			                                        userData.get_type() == sol::type::nil
				                                        ? nullptr
				                                        : userData.as<void*>());
			return std::make_tuple(result, str);
		};
	classImGui["InputTextWithHint"] = [
		](const char *label, const char *hint, std::string str, const int flags, const sol::object &callback,
		  const sol::object &userData) {
			bool result = ImGui::InputTextWithHint(label, hint, &str, flags,
			                                       callback.get_type() == sol::type::nil
				                                       ? nullptr
				                                       : callback.as<ImGuiInputTextCallback>(),
			                                       userData.get_type() == sol::type::nil
				                                       ? nullptr
				                                       : userData.as<void*>());
			return std::make_tuple(result, str);
		};
	state["getCurrentWindowGImgui"] = []() -> ImGuiWindow* {
		return GImGui->CurrentWindow;
	};
}
