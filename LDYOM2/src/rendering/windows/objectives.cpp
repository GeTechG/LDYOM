#define IMGUI_DEFINE_MATH_OPERATORS
#include "objectives.h"
#include "logger.h"
#include <fa_icons.h>
#include <imgui_internal.h>
#include <imgui_widgets/imgui_widgets.h>
#include <localization.h>
#include <objectives_manager.h>
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
		auto emptySpaceRect =
			ImRect(currentWindow->DC.CursorPos, currentWindow->DC.CursorPos + ImVec2(ImGui::GetContentRegionAvail().x,
		                                                                             ImGui::GetContentRegionAvail().y));

		if (ImGui::BeginDragDropTargetCustom(emptySpaceRect, currentWindow->ID)) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("OBJECTIVE_DND")) {
				int draggedIdx = *static_cast<int*>(payload->Data);
				ObjectivesManager::instance().moveObjective(draggedIdx, objectives.size() - 1);
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
					window->m_indexToRemove = i;
				}

				ImGui::EndPopup();
			}

			// TODO: MAYBE PROBLEM WITH POINTERS
			static int lastPtr = 0;
			if (&window->m_renameBuffer == NULL) {
				LDYOM_INFO("Window pointer: {}", (void*)window);
				LDYOM_INFO("Last pointer: {}", (void*)lastPtr);
			}
			lastPtr = (int)window;
			if (openRenamePopupIndex) {
				ImGui::OpenPopup(renamePopupId);
				window->m_renameBuffer = objectives[i].name;
			}

			if (ImGui::RenamePopup(renamePopupId, &window->m_renameBuffer)) {
				if (!window->m_renameBuffer.empty()) {
					auto& objective = ObjectivesManager::instance().getObjectiveMutable(i);
					objective.name = window->m_renameBuffer;
					window->m_renameBuffer.clear();
				}
			}

			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
				ImGui::SetDragDropPayload("OBJECTIVE_DND", &i, sizeof(int));
				ImGui::Text("%s", objectives[i].name.c_str());
				ImGui::EndDragDropSource();
			}

			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("OBJECTIVE_DND")) {
					int draggedIdx = *static_cast<int*>(payload->Data);
					if (draggedIdx != i) {
						ObjectivesManager::instance().moveObjective(draggedIdx, i);

						if (window->m_selectedObjectiveIndex == draggedIdx) {
							window->m_selectedObjectiveIndex = i;
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
		ImGui::SetNextWindowPos(ImGui::GetIO().DisplaySize, ImGuiCond_Always, ImVec2(1, 1));
		if (ImGui::Begin("##objective", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize)) {
			auto& selectedObjective =
				ObjectivesManager::instance().getObjectiveMutable(window->m_selectedObjectiveIndex);
			selectedObjective.renderEditor();
		}
		ImGui::End();
	}

	if (window->m_indexToRemove != -1) {
		auto state = ImGui::ConfirmDialog(_("objectives.remove_title").c_str(), _("objectives.remove_message").c_str());
		if (state == 1) {
			ObjectivesManager::instance().removeObjective(window->m_indexToRemove);
			window->m_selectedObjectiveIndex =
				std::min(window->m_selectedObjectiveIndex, static_cast<int>(objectives.size() - 1));
			window->m_indexToRemove = -1;
		} else if (state == 0) {
			window->m_indexToRemove = -1;
		}
	}
}

ObjectivesWindow::ObjectivesWindow()
	: Window("objectives.title") {
	setSize(200, 360);
	setPosition(0, 0);
	setPivot(0, 0);
	setFlags(ImGuiWindowFlags_NoTitleBar);
	setRenderCallback<ObjectivesWindow>(renderContent);
	m_renameBuffer = "";
}