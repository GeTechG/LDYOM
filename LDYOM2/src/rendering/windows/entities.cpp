#define IMGUI_DEFINE_MATH_OPERATORS
#include "entities.h"
#include "logger.h"
#include <entities_manager.h>
#include <fa_icons.h>
#include <imgui_internal.h>
#include <imgui_widgets/imgui_widgets.h>
#include <localization.h>
#include <scenes_manager.h>
#include <utils/imgui_configurate.h>
#include <window_manager.h>

void EntitiesWindow::renderContent(EntitiesWindow* window) {
	const ImVec2 screenScale = ImGuiConfigurate::getScreenScale();
	const float spacing = ImGui::GetStyle().ItemSpacing.x;
	const float buttonSize = ImGui::GetFrameHeight() * 1.5f; // Используем для квадратных кнопок
	const ImVec2 squareButtonSize(buttonSize, buttonSize);
	const ImVec2 availContentSize = ImGui::GetContentRegionAvail();

	auto& entities = ScenesManager::instance().getCurrentScene().entities;

	if (ImGui::BeginChild("TopButtons", ImVec2(-1.0f, buttonSize), false)) {
		if (ImGui::Button(ICON_FA_ARROW_TURN_LEFT, squareButtonSize)) {
			window->close();
			WindowManager::instance().openWindow("main_menu");
		}

		ImGui::SameLine();

		if (ImGui::Button(ICON_FA_PLUS_LARGE, squareButtonSize)) {
			EntitiesManager::instance().addNewEntityFromTemplate("actor");
		}
	}
	ImGui::EndChild();

	ImGui::Separator();

	const float remainingHeight = availContentSize.y - buttonSize - ImGui::GetStyle().ItemSpacing.y * 2;
	if (ImGui::BeginChild("EntitiesList", ImVec2(-1.0f, remainingHeight), true)) {

		ImGuiWindow* currentWindow = ImGui::GetCurrentWindow();
		auto emptySpaceRect =
			ImRect(currentWindow->DC.CursorPos, currentWindow->DC.CursorPos + ImVec2(ImGui::GetContentRegionAvail().x,
		                                                                             ImGui::GetContentRegionAvail().y));

		if (ImGui::BeginDragDropTargetCustom(emptySpaceRect, currentWindow->ID)) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY_DND")) {
				int draggedIdx = *static_cast<int*>(payload->Data);
				EntitiesManager::instance().moveEntity(draggedIdx, entities.size() - 1);
			}
			ImGui::EndDragDropTarget();
		}

		for (int i = 0; i < static_cast<int>(entities.size()); i++) {
			ImGui::PushID(i);

			bool isSelected = (i == window->m_selectedEntityIndex);

			if (isSelected) {
				ImGui::PushStyleColor(ImGuiCol_Header, ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));
				ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered));
			}

			if (ImGui::Selectable(entities[i].name.c_str(), isSelected, ImGuiSelectableFlags_AllowItemOverlap)) {
				window->m_selectedEntityIndex = i;
			}

			if (isSelected) {
				ImGui::PopStyleColor(2);
			}

			char renamePopupId[32];
			sprintf(renamePopupId, "rename_popup_%d", i);

			bool openRenamePopupIndex = false;
			if (ImGui::BeginPopupContextItem("entity_context_menu")) {
				if (ImGui::MenuItem(_("entities.rename", ICON_FA_I_CURSOR).c_str())) {
					openRenamePopupIndex = true;
				}

				if (ImGui::MenuItem(_("entities.delete", ICON_FA_TRASH).c_str())) {
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
				window->m_renameBuffer = entities[i].name;
			}

			if (ImGui::RenamePopup(renamePopupId, &window->m_renameBuffer)) {
				if (!window->m_renameBuffer.empty()) {
					auto& entity = EntitiesManager::instance().getUnsafeEntity(i);
					entity.name = window->m_renameBuffer;
					window->m_renameBuffer.clear();
				}
			}

			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
				ImGui::SetDragDropPayload("ENTITY_DND", &i, sizeof(int));
				ImGui::Text("%s", entities[i].name.c_str());
				ImGui::EndDragDropSource();
			}

			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY_DND")) {
					int draggedIdx = *static_cast<int*>(payload->Data);
					if (draggedIdx != i) {
						EntitiesManager::instance().moveEntity(draggedIdx, i);

						if (window->m_selectedEntityIndex == draggedIdx) {
							window->m_selectedEntityIndex = i;
						} else if (window->m_selectedEntityIndex > draggedIdx && window->m_selectedEntityIndex <= i) {
							window->m_selectedEntityIndex--;
						} else if (window->m_selectedEntityIndex >= i && window->m_selectedEntityIndex < draggedIdx) {
							window->m_selectedEntityIndex++;
						}
					}
				}
				ImGui::EndDragDropTarget();
			}

			ImGui::PopID();
		}
	}
	ImGui::EndChild();

	if (window->m_selectedEntityIndex >= 0 && window->m_selectedEntityIndex < static_cast<int>(entities.size())) {
		EntitiesWindow::renderEntity(window, entities[window->m_selectedEntityIndex], window->m_selectedEntityIndex);
	}

	if (window->m_indexToRemove != -1) {
		auto state = ImGui::ConfirmDialog(_("entities.remove_title").c_str(), _("entities.remove_message").c_str());
		if (state == 1) {
			EntitiesManager::instance().removeEntity(window->m_indexToRemove);
			window->m_selectedEntityIndex =
				std::min(window->m_selectedEntityIndex, static_cast<int>(entities.size() - 1));
			window->m_indexToRemove = -1;
		} else if (state == 0) {
			window->m_indexToRemove = -1;
		}
	}
}

void EntitiesWindow::renderEntity(EntitiesWindow* window, const Entity& entity, int i) {
	ImGui::SetNextWindowPos(ImGui::GetIO().DisplaySize, ImGuiCond_Always, ImVec2(1, 1));
	if (ImGui::Begin("##entity", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize)) {
		if (ImGui::BeginChild("EntityContentScroll", ImVec2(300.0f, 500.0f) * SCL_PX, ImGuiChildFlags_AutoResizeX)) {
			if (ImGui::CollapsingHeader("Entity", ImGuiTreeNodeFlags_DefaultOpen)) {
				if (ImGui::BeginChild("EntityContent", ImVec2(),
				                      ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_Border)) {
					auto availableWidth = ImGui::GetContentRegionAvail().x;
					// Position
					ImGui::Text("Position");
					ImGui::SameLine(availableWidth * 0.45f);
					std::array<float, 3> position(entity.position);
					ImGui::SetNextItemWidth(-1.f);
					if (ImGui::InputFloat3("##pos", position.data())) {
						EntitiesManager::instance().getUnsafeEntity(i).position = position;
					}

					// Rotation
					ImGui::Text("Rotation");
					ImGui::SameLine(availableWidth * 0.45f);
					std::array<float, 3> rotation(entity.rotation);
					ImGui::SetNextItemWidth(-1.f);
					if (ImGui::InputFloat3("##rot", rotation.data())) {
						EntitiesManager::instance().getUnsafeEntity(i).rotation = rotation;
					}

					// Scale
					ImGui::Text("Scale");
					ImGui::SameLine(availableWidth * 0.45f);
					std::array<float, 3> scale(entity.scale);
					ImGui::SetNextItemWidth(-1.f);
					if (ImGui::InputFloat3("##scale", scale.data())) {
						EntitiesManager::instance().getUnsafeEntity(i).scale = scale;
					}
				}
				ImGui::EndChild();
			}

			// Render components
			auto& components = entity.components.data;
			for (size_t j = 0; j < components.size(); j++) {
				const auto& component = components[j];
				ImGui::PushID(static_cast<int>(j));

				if (ImGui::CollapsingHeader(component->name.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
					component->editorRender();
				}

				ImGui::PopID();
			}

			ImGui::Dummy(ImVec2(0, 10 * (SCL_PX).y));

			if (ImGui::Button("Add Component", ImVec2(-1.f, 0))) {
				// Open component selection menu
			}

			ImGui::EndChild(); // Закрытие дочернего контейнера
		}
		ImGui::End();
	}
}

EntitiesWindow::EntitiesWindow()
	: Window("entities") {
	setSize(200, 360);
	setPosition(0, 0);
	setPivot(0, 0);
	setFlags(ImGuiWindowFlags_NoTitleBar);
	setRenderCallback<EntitiesWindow>(renderContent);
	m_renameBuffer = "";
}