#pragma once
#include "imgui.h"
#include <string>

namespace ImGui {
bool TextSelectable(const char* label, bool selected = false, const ImVec2& size = ImVec2(0, 0));
bool TextButton(const char* label, const ImVec2& size = ImVec2(0, 0));
void TextGTAColored(const char* label);
bool SelectableGTAColored(const char* label, bool selected, ImGuiSelectableFlags flags = 0,
                          const ImVec2& size = ImVec2(0, 0));
bool RenamePopup(const char* popupId, char* buffer, size_t bufferSize);
bool RenamePopup(const char* popupId, std::string* buffer);

/**
 * @brief Creates a modal confirmation dialog that asks user to confirm an action
 *
 * This widget creates a centered modal popup window with a confirmation message
 * and two buttons: one for confirming and one for canceling the action.
 * The window cannot be closed by clicking outside it.
 *
 * @param title Title displayed in the window's title bar
 * @param message The confirmation question or message displayed to the user
 * @param yesButtonText Text for the confirmation button (default: "Yes")
 * @param noButtonText Text for the cancellation button (default: "No")
 *
 * @return int Return value indicating the user's response:
 *         -1: Dialog is still open, no response yet
 *          0: User declined (pressed "No" or closed the window)
 *          1: User confirmed (pressed "Yes")
 */
int ConfirmDialog(const char* title, const char* message, const char* yesButtonText = "yes",
                  const char* noButtonText = "no");
} // namespace ImGui
