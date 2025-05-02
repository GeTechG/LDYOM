#include "imgui_widgets.h"
#include <algorithm>
#include <string>

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

void ImGui::TextGTAColored(const char* label) {
	bool isHighlighted = false;
	ImVec4 currentColor = ImGui::GetStyle().Colors[ImGuiCol_Text]; // Цвет по умолчанию
	size_t len = strlen(label);

	std::string textChunk;

	for (size_t i = 0; i < len; ++i) {
		if (i + 2 < len && label[i] == '~' && label[i + 2] == '~') {
			if (!textChunk.empty()) {
				ImGui::PushStyleColor(ImGuiCol_Text, currentColor);
				ImGui::TextUnformatted(textChunk.c_str());
				ImGui::PopStyleColor();
				ImGui::SameLine(0.0f, 0.0f);
				textChunk.clear();
			}

			switch (label[i + 1]) {
			case 'w':
				currentColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
				isHighlighted = false;
				break; // White
			case 'l':
				currentColor = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
				isHighlighted = false;
				break; // Black
			case 'r':
				currentColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
				isHighlighted = false;
				break; // Red
			case 'g':
				currentColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
				isHighlighted = false;
				break; // Green
			case 'b':
				currentColor = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);
				isHighlighted = false;
				break; // Blue
			case 'y':
				currentColor = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
				isHighlighted = false;
				break; // Yellow
			case 'p':
				currentColor = ImVec4(0.75f, 0.0f, 0.75f, 1.0f);
				isHighlighted = false;
				break; // Purple
			case 's':
				currentColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
				isHighlighted = false;
				break; // White
			case 'h':
				isHighlighted = true;
				if (!isHighlighted) {
					currentColor.x = std::min(1.0f, currentColor.x + 0.5f);
					currentColor.y = std::min(1.0f, currentColor.y + 0.5f);
					currentColor.z = std::min(1.0f, currentColor.z + 0.5f);
				}
				break;
			default:
				break;
			}
			i += 2;
		} else {
			textChunk += label[i];
		}
	}

	if (!textChunk.empty()) {
		ImGui::PushStyleColor(ImGuiCol_Text, currentColor);
		ImGui::TextUnformatted(textChunk.c_str());
		ImGui::PopStyleColor();
	}
}

bool ImGui::SelectableGTAColored(const char* label, bool selected, ImGuiSelectableFlags flags, const ImVec2& size) {
	ImGui::BeginGroup();

	ImVec2 startPos = ImGui::GetCursorPos();

	bool isClicked = ImGui::Selectable("##hidden", selected, flags, size);

	ImGui::SetCursorPos(startPos);

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
	TextGTAColored(label);
	ImGui::PopStyleVar();

	ImGui::EndGroup();
	return isClicked;
}
