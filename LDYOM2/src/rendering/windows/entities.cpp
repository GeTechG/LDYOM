#define IMGUI_DEFINE_MATH_OPERATORS
#include "entities.h"
#include "logger.h"
#include <algorithm>
#include <angles.h>
#include <component.h>
#include <components_manager.h>
#include <entities_manager.h>
#include <fa_icons.h>
#include <glm/ext/quaternion_float.hpp>
#include <glm/gtc/quaternion.hpp>
#include <imgui_internal.h>
#include <imgui_widgets/imgui_widgets.h>
#include <localization.h>
#include <scenes_manager.h>
#include <utils/imgui_configurate.h>
#include <window_manager.h>

bool EntitiesWindow::filterByType(Entity& entity) {
	switch (m_windowType) {
		case EntitiesWindowType_Actor: return entity.hasComponent("actor");
		case EntitiesWindowType_Vehicle: return entity.hasComponent("vehicle");
		case EntitiesWindowType_Object: return entity.hasComponent("object");
		case EntitiesWindowType_Pickup: return entity.hasComponent("pickup");
		case EntitiesWindowType_Firework: return entity.hasComponent("firework");
		default: return false;
	}
}

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
			switch (window->m_windowType) {
				case EntitiesWindowType_Actor:
					{
						EntitiesManager::instance().addNewEntityFromTemplate("actor");
						break;
					}
				case EntitiesWindowType_Vehicle:
					{
						EntitiesManager::instance().addNewEntityFromTemplate("vehicle");
						break;
					}
				case EntitiesWindowType_Object:
					{
						EntitiesManager::instance().addNewEntityFromTemplate("object");
						break;
					}
				case EntitiesWindowType_Pickup:
					{
						EntitiesManager::instance().addNewEntityFromTemplate("pickup");
						break;
					}
				case EntitiesWindowType_Firework:
					{
						EntitiesManager::instance().addNewEntityFromTemplate("firework");
						break;
					}
			}
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

			if (!window->filterByType(*entities[i])) {
				continue; // Skip entities that don't match the filter
			}

			ImGui::PushID(i);

			bool isSelected = (i == window->m_selectedEntityIndex);

			if (isSelected) {
				ImGui::PushStyleColor(ImGuiCol_Header, ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));
				ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered));
			}

			if (ImGui::Selectable(entities[i]->name.c_str(), isSelected, ImGuiSelectableFlags_AllowItemOverlap)) {
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
				window->m_renameBuffer = entities[i]->name;
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
				ImGui::Text("%s", entities[i]->name.c_str());
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
		EntitiesWindow::renderEntity(window, *entities[window->m_selectedEntityIndex], window->m_selectedEntityIndex);
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
					if (ImGui::DragFloat3("##pos", position.data(), 0.1f, -10000.0f, 10000.0f)) {
						auto& entity = EntitiesManager::instance().getUnsafeEntity(i);
						entity.position = position;
						entity.updateSetTransformCallbacks();
					}

					// Rotation
					ImGui::Text("Rotation");
					ImGui::SameLine(availableWidth * 0.45f);

					static CQuaternion lastQ;
					const auto q = entity.rotation;
					const glm::quat quaternion(q.real, q.imag.x, q.imag.y, q.imag.z);
					const glm::vec3 eulerAngles = glm::eulerAngles(quaternion);

					// rotations
					static std::array<float, 3> eularRot = {0, 0, 0};
					if (abs(q.real - lastQ.real) > FLT_EPSILON || abs(q.imag.x - lastQ.imag.x) > FLT_EPSILON ||
					    abs(q.imag.y - lastQ.imag.y) > FLT_EPSILON || abs(q.imag.z - lastQ.imag.z) > FLT_EPSILON) {
						eularRot[0] = glm::degrees(eulerAngles.x);
						eularRot[1] = glm::degrees(eulerAngles.y);
						eularRot[2] = glm::degrees(eulerAngles.z);
						lastQ = q;
					}
					ImGui::SetNextItemWidth(-1.f);
					if (ImGui::DragFloat3("##rot", eularRot.data(), 0.1f, -180.0f, 180.0f)) {
						auto& entity = EntitiesManager::instance().getUnsafeEntity(i);
						entity.rotation.Set(RAD(eularRot[1]), RAD(eularRot[2]), RAD(eularRot[0]));
						entity.updateSetTransformCallbacks();
					}

					// Scale
					ImGui::Text("Scale");
					ImGui::SameLine(availableWidth * 0.45f);
					std::array<float, 3> scale(entity.scale);
					ImGui::SetNextItemWidth(-1.f);
					if (ImGui::DragFloat3("##scale", scale.data(), 0.1f, -10000.0f, 10000.0f)) {
						auto& entity = EntitiesManager::instance().getUnsafeEntity(i);
						entity.scale = scale;
						entity.updateSetTransformCallbacks();
					}
				}
				ImGui::EndChild();
			}

			// Render components
			auto& components = entity.getComponents();
			for (size_t j = 0; j < components.size(); j++) {
				const auto& component = components[j];
				ImGui::PushID(static_cast<int>(j));

				// Draw the collapsing header with AllowOverlap flag
				bool headerOpen = ImGui::CollapsingHeader(
					component->getName().c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowOverlap);
				// Position menu button on the right side of the header
				float buttonSize = ImGui::GetFrameHeight();
				ImGui::SameLine(ImGui::GetContentRegionAvail().x - buttonSize);

				// Style the menu button to look more integrated
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 2.0f));
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0)); // Transparent background
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_HeaderHovered));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImGui::GetStyleColorVec4(ImGuiCol_HeaderActive));

				// Square menu button with three dots
				char menuButtonId[32];
				sprintf(menuButtonId, "menu_btn_%zu", j);
				if (ImGui::Button(ICON_FA_ELLIPSIS_VERTICAL, ImVec2(buttonSize, buttonSize))) {
					char menuPopupId[32];
					sprintf(menuPopupId, "component_menu_%zu", j);
					ImGui::OpenPopup(menuPopupId);
				}

				ImGui::PopStyleColor(3);
				ImGui::PopStyleVar(); // Context menu popup
				char menuPopupId[32];
				sprintf(menuPopupId, "component_menu_%zu", j);
				if (ImGui::BeginPopup(menuPopupId)) {
					const auto& componentName = component->getType();
					const auto isSpecial =
						ComponentsManager::instance().getComponentBuilders().at(componentName).isSpecial;
					if (ImGui::MenuItem(_("entities.component_remove", ICON_FA_TRASH).c_str(), nullptr, nullptr,
					                    !isSpecial)) {
						window->m_pendingComponentOperations.push_back(
							{EntitiesWindow::PendingComponentOperation::Remove, j});
					}

					if (ImGui::MenuItem(_("entities.component_duplicate", ICON_FA_COPY).c_str())) {
						window->m_pendingComponentOperations.push_back(
							{EntitiesWindow::PendingComponentOperation::Duplicate, j});
					}

					ImGui::Separator();

					if (ImGui::MenuItem(_("entities.component_move_up", ICON_FA_ARROW_UP).c_str(), nullptr, false,
					                    j > 0)) {
						window->m_pendingComponentOperations.push_back(
							{EntitiesWindow::PendingComponentOperation::MoveUp, j});
					}

					if (ImGui::MenuItem(_("entities.component_move_down", ICON_FA_ARROW_DOWN).c_str(), nullptr, false,
					                    j < components.size() - 1)) {
						window->m_pendingComponentOperations.push_back(
							{EntitiesWindow::PendingComponentOperation::MoveDown, j});
					}

					ImGui::EndPopup();
				} // Render component content if header is open
				if (headerOpen) {
					component->editorRender();
				}
				ImGui::PopID();
			}

			// Обработка отложенных операций с компонентами
			if (!window->m_pendingComponentOperations.empty()) {
				auto& entity = EntitiesManager::instance().getUnsafeEntity(i);

				// Сортируем операции по индексу в обратном порядке для корректной обработки удаления
				std::sort(window->m_pendingComponentOperations.begin(), window->m_pendingComponentOperations.end(),
				          [](const EntitiesWindow::PendingComponentOperation& a,
				             const EntitiesWindow::PendingComponentOperation& b) {
							  // Удаления должны идти в обратном порядке по индексу
							  if (a.type == EntitiesWindow::PendingComponentOperation::Remove &&
					              b.type == EntitiesWindow::PendingComponentOperation::Remove) {
								  return a.componentIndex > b.componentIndex;
							  }
							  // Остальные операции в прямом порядке
							  return a.componentIndex < b.componentIndex;
						  });

				for (const auto& operation : window->m_pendingComponentOperations) {
					switch (operation.type) {
						case EntitiesWindow::PendingComponentOperation::Remove:
							entity.removeComponent(operation.componentIndex);
							break;
						case EntitiesWindow::PendingComponentOperation::Duplicate:
							entity.duplicateComponent(operation.componentIndex);
							break;
						case EntitiesWindow::PendingComponentOperation::MoveUp:
							entity.moveComponentUp(operation.componentIndex);
							break;
						case EntitiesWindow::PendingComponentOperation::MoveDown:
							entity.moveComponentDown(operation.componentIndex);
							break;
					}
				}
				window->m_pendingComponentOperations.clear();
			}

			ImGui::Dummy(ImVec2(0, 10 * (SCL_PX).y));
			if (ImGui::Button(_("entities.add_component").c_str(), ImVec2(-1.f, 0))) {
				WindowManager::instance().openWindow("create_new_component");
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