#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_widgets.h"
#include <algorithm>
#include <imgui_stdlib.h>
#include <localization.h>
#include <string>
#include <utils/imgui_configurate.h>

bool ImGui::TextSelectable(const char* label, bool selected, const ImVec2& size) {
	PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	if (!selected) {
		PushStyleColor(ImGuiCol_Text, GetStyleColorVec4(ImGuiCol_TextDisabled));
	}
	const auto result = Button(label, size);
	PopStyleColor(3);
	if (!selected) {
		PopStyleColor();
	}
	return result;
}

bool ImGui::TextButton(const char* label, const ImVec2& size) {
	PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	const auto result = Button(label, size);
	PopStyleColor(3);
	return result;
}

void ImGui::TextGTAColored(const char* label) {
	bool isHighlighted = false;
	ImVec4 currentColor = GetStyle().Colors[ImGuiCol_Text]; // Default color
	size_t len = strlen(label);

	std::string textChunk;

	for (size_t i = 0; i < len; ++i) {
		if (i + 2 < len && label[i] == '~' && label[i + 2] == '~') {
			if (!textChunk.empty()) {
				PushStyleColor(ImGuiCol_Text, currentColor);
				TextUnformatted(textChunk.c_str());
				PopStyleColor();
				SameLine(0.0f, 0.0f);
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
		PushStyleColor(ImGuiCol_Text, currentColor);
		TextUnformatted(textChunk.c_str());
		PopStyleColor();
	}
}

bool ImGui::SelectableGTAColored(const char* label, bool selected, ImGuiSelectableFlags flags, const ImVec2& size) {
	BeginGroup();

	ImVec2 startPos = GetCursorPos();

	bool isClicked = Selectable("##hidden", selected, flags, size);

	SetCursorPos(startPos);

	PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
	TextGTAColored(label);
	PopStyleVar();

	EndGroup();
	return isClicked;
}

bool ImGui::RenamePopup(const char* popupId, char* buffer, size_t bufferSize) {
	bool renamed = false;
	if (BeginPopup(popupId)) {
		SetKeyboardFocusHere();

		if (InputText("##rename", buffer, bufferSize, ImGuiInputTextFlags_EnterReturnsTrue)) {
			renamed = true;
			CloseCurrentPopup();
		}

		if (IsMouseClicked(0) && !IsItemHovered() &&
		    IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem) == false) {
			CloseCurrentPopup();
		}

		EndPopup();
	}

	return renamed;
}

bool ImGui::RenamePopup(const char* popupId, std::string* buffer) {
	bool renamed = false;
	if (BeginPopup(popupId)) {
		SetKeyboardFocusHere();

		if (InputText("##rename", buffer, ImGuiInputTextFlags_EnterReturnsTrue)) {
			renamed = true;
			CloseCurrentPopup();
		}

		if (IsMouseClicked(0) && !IsItemHovered() &&
		    IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem) == false) {
			CloseCurrentPopup();
		}

		EndPopup();
	}

	return renamed;
}

int ImGui::ConfirmDialog(const char* title, const char* message, const char* yesButtonText, const char* noButtonText) {
	int result = -1;

	if (!IsPopupOpen(title)) {
		OpenPopup(title);
	}

	// Center the dialog on screen
	ImVec2 center = GetMainViewport()->GetCenter();
	SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	// Set compact window size
	SetNextWindowSize(ImVec2(0, 0));

	// Modal popup that cannot be closed by clicking outside
	if (BeginPopupModal(title, nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		Text(message);
		Separator();

		// Add spacing before buttons
		Dummy(ImVec2(0.0f, 5.0f) * SCL_PX);

		// Create block for centering buttons
		float width = GetWindowWidth();
		float buttonWidth = 120.0f * (SCL_PX).x;
		float spacingX = 10.0f * (SCL_PX).x;
		float totalWidth = 2 * buttonWidth + spacingX;
		float offsetX = (width - totalWidth) * 0.5f;

		if (offsetX > 0)
			SetCursorPosX(offsetX);

		if (Button(_(yesButtonText).c_str(), ImVec2(buttonWidth, 0))) {
			result = 1;
			CloseCurrentPopup();
		}

		SameLine(0, spacingX);

		if (Button(_(noButtonText).c_str(), ImVec2(buttonWidth, 0)) || IsKeyPressed(ImGuiKey_Escape)) {
			result = 0;
			CloseCurrentPopup();
		}

		EndPopup();
	} else {
		// If the window was closed by other means
		result = 0;
	}

	return result;
}