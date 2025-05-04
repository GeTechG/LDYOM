#define IMGUI_DEFINE_MATH_OPERATORS
#include "objectives.h"
#include <fa_icons.h>
#include <imgui_internal.h>
#include <imgui_widgets/imgui_widgets.h>
#include <localization.h>
#include <scenes_manager.h>
#include <utils/imgui_configurate.h>
#include <window_manager.h>

void ObjectivesWindow::renderContent(ObjectivesWindow* window) {
	const ImVec2 screenScale = ImGuiConfigurate::getScreenScale();
	const float spacing = ImGui::GetStyle().ItemSpacing.x;
	const float buttonSize = ImGui::GetFrameHeight() * 1.5f; // Используем для квадратных кнопок
	const ImVec2 squareButtonSize(buttonSize, buttonSize);
	const ImVec2 availContentSize = ImGui::GetContentRegionAvail();

	auto& objectives = ScenesManager::instance().getCurrentScene().objectives.data;

	if (ImGui::BeginChild("TopButtons", ImVec2(-1.0f, buttonSize), false)) {
		if (ImGui::Button(ICON_FA_ARROW_TURN_LEFT, squareButtonSize)) {
			window->close();
			WindowManager::instance().openWindow("main_menu");
		}

		ImGui::SameLine();

		if (ImGui::Button(ICON_FA_PLUS_LARGE, squareButtonSize)) {
			WindowManager::instance().openWindow("create_new_objective");
		}
	}
	ImGui::EndChild();

	ImGui::Separator();

	const float remainingHeight = availContentSize.y - buttonSize - ImGui::GetStyle().ItemSpacing.y * 2;
	if (ImGui::BeginChild("ObjectivesList", ImVec2(-1.0f, remainingHeight), true)) {

		ImGuiWindow* currentWindow = ImGui::GetCurrentWindow();
		ImRect emptySpaceRect =
			ImRect(currentWindow->DC.CursorPos, currentWindow->DC.CursorPos + ImVec2(ImGui::GetContentRegionAvail().x,
		                                                                             ImGui::GetContentRegionAvail().y));

		if (ImGui::BeginDragDropTargetCustom(emptySpaceRect, currentWindow->ID)) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("OBJECTIVE_DND")) {
				int draggedIdx = *(int*)payload->Data;
				Objective temp = objectives[draggedIdx];
				objectives.erase(objectives.begin() + draggedIdx);
				objectives.insert(objectives.begin(), temp);
			}
			ImGui::EndDragDropTarget();
		}

		for (int i = 0; i < static_cast<int>(objectives.size()); i++) {
			ImGui::PushID(i);

			bool isSelected = (i == window->m_selectedObjectiveIndex);

			if (isSelected) {
				ImGui::PushStyleColor(ImGuiCol_Header, ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));
				ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered));
			}

			if (ImGui::Selectable(objectives[i].name.c_str(), isSelected, ImGuiSelectableFlags_AllowItemOverlap)) {
				window->m_selectedObjectiveIndex = i;
			}

			if (isSelected) {
				ImGui::PopStyleColor(2);
			}

			char renamePopupId[32];
			sprintf(renamePopupId, "rename_popup_%d", i);

			bool openRenamePopupIndex = false;
			if (ImGui::BeginPopupContextItem("objective_context_menu")) {
				if (ImGui::MenuItem(_("objectives.rename", ICON_FA_I_CURSOR).c_str())) {
					openRenamePopupIndex = true;
				}

				if (ImGui::MenuItem(_("objectives.delete", ICON_FA_TRASH).c_str())) {
					objectives.erase(objectives.begin() + i);
					if (window->m_selectedObjectiveIndex == i) {
						window->m_selectedObjectiveIndex = -1;
					} else if (window->m_selectedObjectiveIndex > i) {
						window->m_selectedObjectiveIndex--;
					}
				}

				ImGui::EndPopup();
			}

			if (openRenamePopupIndex) {
				ImGui::OpenPopup(renamePopupId);
			}

			ImGui::RenamePopup(renamePopupId, &objectives[i].name);

			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
				ImGui::SetDragDropPayload("OBJECTIVE_DND", &i, sizeof(int));
				ImGui::Text("%s", objectives[i].name.c_str());
				ImGui::EndDragDropSource();
			}

			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("OBJECTIVE_DND")) {
					int draggedIdx = *(int*)payload->Data;
					if (draggedIdx != i) {
						Objective temp = objectives[draggedIdx];

						objectives.erase(objectives.begin() + draggedIdx);
						int targetIdx = (draggedIdx < i) ? i - 1 : i;
						objectives.insert(objectives.begin() + targetIdx, temp);

						if (window->m_selectedObjectiveIndex == draggedIdx) {
							window->m_selectedObjectiveIndex = targetIdx;
						} else if (window->m_selectedObjectiveIndex > draggedIdx &&
						           window->m_selectedObjectiveIndex <= i) {
							window->m_selectedObjectiveIndex--;
						} else if (window->m_selectedObjectiveIndex >= i &&
						           window->m_selectedObjectiveIndex < draggedIdx) {
							window->m_selectedObjectiveIndex++;
						}
					}
				}
				ImGui::EndDragDropTarget();
			}

			ImGui::PopID();
		}
	}
	ImGui::EndChild();

	if (window->m_selectedObjectiveIndex >= 0 &&
	    window->m_selectedObjectiveIndex < static_cast<int>(objectives.size())) {
		ImGui::SetNextWindowPos(ImGui::GetIO().DisplaySize, ImGuiCond_FirstUseEver, ImVec2(1, 1));
		if (ImGui::Begin("##objective", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize)) {
			auto& selectedObjective = objectives[window->m_selectedObjectiveIndex];
			selectedObjective.renderEditor();
		}
		ImGui::End();
	}
}

ObjectivesWindow::ObjectivesWindow()
	: Window("objectives.title") {
	setSize(200, 360);
	setPosition(0, 0);
	setPivot(0, 0);
	setFlags(ImGuiWindowFlags_NoTitleBar);
	setRenderCallback<ObjectivesWindow>(ObjectivesWindow::renderContent);
}