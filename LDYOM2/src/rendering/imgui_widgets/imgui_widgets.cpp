#include "imgui_widgets.h"

bool ImGui::TextSelectable(const char* label, bool selected, const ImVec2& size) {
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	if (!selected) {
		ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
	}
	const auto result = ImGui::Button(label, size);
	ImGui::PopStyleColor(3);
	if (!selected) {
		ImGui::PopStyleColor();
	}
	return result;
}

bool ImGui::TextButton(const char* label, const ImVec2& size) {
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	const auto result = ImGui::Button(label, size);
	ImGui::PopStyleColor(3);
	return result;
}
