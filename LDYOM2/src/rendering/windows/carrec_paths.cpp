#define IMGUI_DEFINE_MATH_OPERATORS
#include "carrec_paths.h"
#include <CModelInfo.h>
#include <CVehicleModelInfo.h>
#include <components_manager.h>
#include <data/components/vehicle.h>
#include <data/entity.h>
#include <entities_manager.h>
#include <fa_icons.h>
#include <fmt/core.h>
#include <imgui_widgets/imgui_widgets.h>
#include <localization.h>
#include <logger.h>
#include <models_manager.h>
#include <popups/vehicle_selector.h>
#include <rendering/in_game/carrec_path_editor.h>
#include <scenes_manager.h>
#include <settings.h>
#include <utils/carrec_paths_service.h>
#include <utils/imgui_configurate.h>
#include <vector>

void CarrecPathsWindow::renderContent(CarrecPathsWindow* window) {
	auto& paths = CarrecPathsService::instance().getPaths();

	// Header section with improved spacing
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(12, 6) * SCL_PX);
	if (ImGui::Button(_("carrec_paths.new_recording", ICON_FA_PLUS).c_str(), ImVec2(180 * (SCL_PX).x, 0))) {
		window->m_showSelectVehiclePopup = true;
		ImGui::OpenPopup("SelectVehicleForNewRecording");
	}
	ImGui::PopStyleVar();

	ImGui::SameLine(0, 15 * (SCL_PX).x);

	// Paths count badge с фоном
	auto badgeColor = ImGui::GetStyleColorVec4(ImGuiCol_ChildBg);
	badgeColor.w *= 1.2f;
	ImGui::PushStyleColor(ImGuiCol_ChildBg, badgeColor);
	ImGui::BeginChild("PathCountBadge", ImVec2(150 * (SCL_PX).x, 28 * (SCL_PX).y), true, ImGuiWindowFlags_NoScrollbar);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3 * (SCL_PX).y);
	ImGui::TextColored(ImGui::GetStyleColorVec4(ImGuiCol_Text), _("carrec_paths.paths_count", paths.size()).c_str());
	ImGui::EndChild();
	ImGui::PopStyleColor();

	// Select Vehicle popup for new recording
	if (window->m_showSelectVehiclePopup) {
		window->selectVehiclePopup(
			"SelectVehicleForNewRecording", &window->m_showSelectVehiclePopup, [window, &paths](int model) {
				// Create new path
				auto* newPath = CarrecPathsService::instance().createNewPath();
				newPath->setVehicleModel(model);
				// Gather preview paths
				std::vector<CarrecPath*> previewPaths;
				std::vector<bool> aiRerecord;
				for (const auto& idx : window->m_useInPreview) {
					previewPaths.push_back(&paths[idx]);
					aiRerecord.push_back(window->m_useAIRerecord.contains(idx));
				}

				// Open recording editor
				CarrecPathEditor::openEditor(
					0, // type: 0 = Record new
					*newPath, previewPaths, aiRerecord,
					[newPath, previewPaths](bool isAccepted, std::vector<CVehicleStateEachFrame> recordedPath,
			                                const std::map<int, std::vector<CVehicleStateEachFrame>>& rerecordedPaths) {
						if (isAccepted && !recordedPath.empty()) {
							newPath->clearFrames();
							for (const auto& frame : recordedPath) {
								newPath->addFrame(frame);
							}
							// Update AI re-recorded paths
							for (const auto& [index, frames] : rerecordedPaths) {
								previewPaths[index]->clearFrames();
								for (const auto& frame : frames) {
									previewPaths[index]->addFrame(frame);
								}
							}
							LDYOM_INFO("Created new path '{}' with {} frames", newPath->getName(), recordedPath.size());
						} else {
							// User cancelled, delete the path
							CarrecPathsService::instance().deletePath(newPath->getId());
							LDYOM_INFO("Cancelled creation of path '{}'", newPath->getName());
						}
					});
			});
	}

	// Визуальный разделитель
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_Border).x,
	                                                 ImGui::GetStyleColorVec4(ImGuiCol_Border).y,
	                                                 ImGui::GetStyleColorVec4(ImGuiCol_Border).z, 0.8f));
	ImGui::Separator();
	ImGui::PopStyleColor();
	ImGui::Spacing();
	ImGui::Spacing();

	// Paths list
	if (paths.empty()) {
		// Centered empty state message
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 40 * (SCL_PX).y);
		auto emptyText = _("carrec_paths.no_recorded_paths");
		auto hintText = _("carrec_paths.no_recorded_paths_hint");

		float windowWidth = ImGui::GetContentRegionAvail().x;
		float textWidth = ImGui::CalcTextSize(emptyText.c_str()).x;
		ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
		ImGui::TextDisabled("%s", emptyText.c_str());

		ImGui::Spacing();
		ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + windowWidth);
		ImGui::TextWrapped("%s", hintText.c_str());
		ImGui::PopTextWrapPos();
	} else {
		// Table с улучшенным spacing
		const ImGuiTableFlags tableFlags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable |
		                                   ImGuiTableFlags_ScrollY | ImGuiTableFlags_HighlightHoveredColumn;

		// Улучшенный padding для таблицы
		ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(8, 6) * SCL_PX);

		if (ImGui::BeginTable("PathsTable", 7, tableFlags, ImVec2(0, 300) * SCL_PX)) {
			ImGui::TableSetupColumn(_("carrec_path.name").c_str(), ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn(_("components.vehicle.name").c_str(), ImGuiTableColumnFlags_WidthFixed,
			                        110.0f * (SCL_PX).x);
			ImGui::TableSetupColumn(_("carrec_path.frames").c_str(), ImGuiTableColumnFlags_WidthFixed,
			                        70.0f * (SCL_PX).x);
			ImGui::TableSetupColumn(_("carrec_path.duration").c_str(), ImGuiTableColumnFlags_WidthFixed,
			                        80.0f * (SCL_PX).x);
			ImGui::TableSetupColumn(_("carrec_path.preview").c_str(), ImGuiTableColumnFlags_WidthFixed,
			                        70.0f * (SCL_PX).x);
			ImGui::TableSetupColumn(_("carrec_path.ai_rerecord").c_str(), ImGuiTableColumnFlags_WidthFixed,
			                        90.0f * (SCL_PX).x);
			ImGui::TableSetupColumn(_("general.actions").c_str(), ImGuiTableColumnFlags_WidthFixed, 60.0f * (SCL_PX).x);
			ImGui::TableSetupScrollFreeze(0, 1);

			// Выделенный header
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImGui::GetStyleColorVec4(ImGuiCol_Header));
			ImGui::TableHeadersRow();
			ImGui::PopStyleColor();

			for (size_t i = 0; i < paths.size(); i++) {
				auto& path = paths[i];
				ImGui::PushID(static_cast<int>(i));

				ImGui::TableNextRow();

				// Name column
				ImGui::TableNextColumn();
				bool isSelected = (window->m_selectedPathIndex == static_cast<int>(i));
				if (ImGui::Selectable(path.getName().c_str(), isSelected,
				                      ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowOverlap)) {
					window->m_selectedPathIndex = static_cast<int>(i);
					window->m_useInPreview.erase(static_cast<int>(i));
					window->m_useAIRerecord.erase(static_cast<int>(i));
				}

				// Context menu on right-click
				char contextMenuId[32];
				sprintf(contextMenuId, "context_menu_%d", static_cast<int>(i));
				if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
					window->m_renameBuffer = path.getName();
					ImGui::OpenPopup(contextMenuId);
				}

				bool openRenamePopup = false;
				if (ImGui::BeginPopup(contextMenuId)) {
					if (ImGui::MenuItem(fmt::format(ICON_FA_PEN " {}", _("general.rename")).c_str())) {
						openRenamePopup = true;
					}
					if (ImGui::MenuItem(fmt::format(ICON_FA_TRASH " {}", _("general.delete")).c_str())) {
						window->m_indexToRemove = static_cast<int>(i);
					}
					ImGui::EndPopup();
				}

				if (openRenamePopup) {
					ImGui::OpenPopup("##rename_popup");
				}

				if (ImGui::RenamePopup("##rename_popup", &window->m_renameBuffer)) {
					if (!window->m_renameBuffer.empty() &&
					    !CarrecPathsService::instance().pathNameExists(window->m_renameBuffer)) {
						path.setName(window->m_renameBuffer);
						LDYOM_INFO("Renamed path to '{}'", window->m_renameBuffer);
					} else if (CarrecPathsService::instance().pathNameExists(window->m_renameBuffer)) {
						LDYOM_WARN("Path name '{}' already exists", window->m_renameBuffer);
					}
				}

				// Vehicle column - показываем название модели и кнопку для смены
				ImGui::TableNextColumn();
				const auto* modelInfo =
					reinterpret_cast<CVehicleModelInfo*>(CModelInfo::GetModelInfo(path.getVehicleModel()));
				const char* vehicleName = modelInfo ? &modelInfo->m_szGameName[0] : "Unknown";

				// Кнопка с иконкой автомобиля
				if (ImGui::SmallButton(fmt::format("{} {}##{}", ICON_FA_CAR, vehicleName, i).c_str())) {
					window->m_editPathIndex = static_cast<int>(i);
					window->m_selectedVehicleModel = path.getVehicleModel();
					window->m_showExistingSelectVehiclePopup = true;
				}
				if (ImGui::IsItemHovered()) {
					ImGui::SetTooltip(_("carrec_paths.change_vehicle").c_str());
				}

				// Frames column
				ImGui::TableNextColumn();
				ImGui::Text("%zu", path.getFrameCount());

				// Duration column
				ImGui::TableNextColumn();
				ImGui::Text("%.1fs", path.getDuration());

				// Preview column
				ImGui::TableNextColumn();
				auto previewToggle = window->m_useInPreview.contains(static_cast<int>(i));
				if (ImGui::Checkbox(fmt::format("##preview{}", i).c_str(), &previewToggle)) {
					if (previewToggle) {
						window->m_useInPreview.insert(static_cast<int>(i));
						if (window->m_selectedPathIndex == static_cast<int>(i)) {
							window->m_selectedPathIndex = -1;
						}
					} else {
						window->m_useInPreview.erase(static_cast<int>(i));
						window->m_useAIRerecord.erase(static_cast<int>(i));
					}
				}
				if (ImGui::IsItemHovered()) {
					ImGui::SetTooltip("%s", _("carrec_path.preview_tooltip").c_str());
				}

				// AI Rerecord column
				ImGui::TableNextColumn();
				auto aiRerecordToggle = window->m_useAIRerecord.contains(static_cast<int>(i));
				if (ImGui::Checkbox(fmt::format("##aiRerecord{}", i).c_str(), &aiRerecordToggle)) {
					if (aiRerecordToggle) {
						window->m_useAIRerecord.insert(static_cast<int>(i));
						window->m_useInPreview.insert(static_cast<int>(i));
						if (window->m_selectedPathIndex == static_cast<int>(i)) {
							window->m_selectedPathIndex = -1;
						}
					} else {
						window->m_useAIRerecord.erase(static_cast<int>(i));
					}
				}
				if (ImGui::IsItemHovered()) {
					ImGui::SetTooltip("%s", _("carrec_path.ai_rerecord_tooltip").c_str());
				}

				// Actions column
				ImGui::TableNextColumn();

				// Кнопка удаления
				if (ImGui::SmallButton(fmt::format("{}##{}", ICON_FA_TRASH, i).c_str())) {
					window->m_indexToRemove = static_cast<int>(i);
				}
				if (ImGui::IsItemHovered()) {
					ImGui::SetTooltip(_("general.delete").c_str());
				}

				ImGui::PopID();
			}

			ImGui::EndTable();
		}

		ImGui::PopStyleVar(); // CellPadding
	}

	// Delete confirmation dialog
	if (window->m_indexToRemove != -1 && window->m_indexToRemove < static_cast<int>(paths.size())) {
		auto& pathToRemove = paths[window->m_indexToRemove];
		auto state = ImGui::ConfirmDialog(_("carrec_paths.delete_path_title").c_str(),
		                                  _("carrec_paths.delete_path_message", pathToRemove.getName()).c_str());
		if (state == 1) {
			LDYOM_INFO("Deleting path '{}'", pathToRemove.getName());
			CarrecPathsService::instance().deletePath(pathToRemove.getId());
			window->m_selectedPathIndex = std::min(window->m_selectedPathIndex, static_cast<int>(paths.size() - 1));
			window->m_indexToRemove = -1;
		} else if (state == 0) {
			window->m_indexToRemove = -1;
		}
	}

	// Визуальный разделитель перед кнопками действий
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_Border).x,
	                                                 ImGui::GetStyleColorVec4(ImGuiCol_Border).y,
	                                                 ImGui::GetStyleColorVec4(ImGuiCol_Border).z, 0.8f));
	ImGui::Separator();
	ImGui::PopStyleColor();
	ImGui::Spacing();
	ImGui::Spacing();

	// Action buttons с улучшенным spacing
	ImGui::BeginDisabled(window->m_selectedPathIndex == -1);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(12, 8) * SCL_PX);
	if (ImGui::Button(_("carrec_paths.play", ICON_FA_PLAY).c_str(), ImVec2(140 * (SCL_PX).x, 0))) {
		auto& selectedPath = paths[window->m_selectedPathIndex];

		std::vector<CarrecPath*> previewPaths;
		std::vector<bool> aiRerecord;
		for (const auto& idx : window->m_useInPreview) {
			previewPaths.push_back(&paths[idx]);
			aiRerecord.push_back(false); // Play mode - no AI rerecord
		}

		CarrecPathEditor::openEditor(1, // type: 1 = Play/Preview
		                             selectedPath, previewPaths, aiRerecord,
		                             [](bool, const std::vector<CVehicleStateEachFrame>&,
		                                const std::map<int, std::vector<CVehicleStateEachFrame>>&) {
										 // No changes on play mode
									 });
	}

	ImGui::SameLine(0, 10 * (SCL_PX).x);
	if (ImGui::Button(_("carrec_paths.rerecord", ICON_FA_PEN).c_str(), ImVec2(140 * (SCL_PX).x, 0))) {
		auto& selectedPath = paths[window->m_selectedPathIndex];

		std::vector<CarrecPath*> previewPaths;
		std::vector<bool> aiRerecord;
		for (const auto& idx : window->m_useInPreview) {
			previewPaths.push_back(&paths[idx]);
			aiRerecord.push_back(window->m_useAIRerecord.contains(idx));
		}

		CarrecPathEditor::openEditor(
			0, // type: 0 = Record/Re-record
			selectedPath, previewPaths, aiRerecord,
			[&selectedPath, previewPaths](bool isAccepted, std::vector<CVehicleStateEachFrame> recordedPath,
		                                  const std::map<int, std::vector<CVehicleStateEachFrame>>& rerecordedPaths) {
				if (isAccepted && !recordedPath.empty()) {
					selectedPath.clearFrames();
					for (const auto& frame : recordedPath) {
						selectedPath.addFrame(frame);
					}
					// Update AI re-recorded paths
					for (const auto& [index, frames] : rerecordedPaths) {
						previewPaths[index]->clearFrames();
						for (const auto& frame : frames) {
							previewPaths[index]->addFrame(frame);
						}
					}
					LDYOM_INFO("Re-recorded path '{}' with {} frames", selectedPath.getName(), recordedPath.size());
				}
			});
	}
	ImGui::PopStyleVar(); // FramePadding
	ImGui::EndDisabled();

	// Vehicle selection popup for existing paths
	if (window->m_showExistingSelectVehiclePopup) {
		if (!ImGui::IsPopupOpen("ExistingSelectVehicle")) {
			ImGui::OpenPopup("ExistingSelectVehicle");
		}
		window->selectVehiclePopup(
			"ExistingSelectVehicle", &window->m_showExistingSelectVehiclePopup, [window, &paths](int model) {
				if (window->m_editPathIndex >= 0 && window->m_editPathIndex < static_cast<int>(paths.size())) {
					paths[window->m_editPathIndex].setVehicleModel(model);
					LDYOM_INFO("Changed path '{}' vehicle to model {}", paths[window->m_editPathIndex].getName(),
				               model);
				}
				window->m_editPathIndex = -1;
			});
	}

	// Info section с визуальным разделением
	ImGui::Spacing();
	ImGui::Spacing();

	// Tips в отдельной секции с иконкой
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_FrameBg).x,
	                                               ImGui::GetStyleColorVec4(ImGuiCol_FrameBg).y,
	                                               ImGui::GetStyleColorVec4(ImGuiCol_FrameBg).z, 0.5f));
	ImGui::BeginChild("TipsSection", ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar);
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 4) * SCL_PX);

	ImGui::TextColored(ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_Text).x * 0.8f,
	                          ImGui::GetStyleColorVec4(ImGuiCol_Text).y * 0.8f,
	                          ImGui::GetStyleColorVec4(ImGuiCol_Text).z * 0.8f, 1.0f),
	                   "%s  %s", ICON_FA_LIGHTBULB, _("carrec_paths.tip_right_click").c_str());

	ImGui::TextColored(ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_Text).x * 0.8f,
	                          ImGui::GetStyleColorVec4(ImGuiCol_Text).y * 0.8f,
	                          ImGui::GetStyleColorVec4(ImGuiCol_Text).z * 0.8f, 1.0f),
	                   "%s  %s", ICON_FA_LIGHTBULB, _("carrec_paths.tip_preview_ai").c_str());

	ImGui::PopStyleVar();
	ImGui::EndChild();
	ImGui::PopStyleColor();
}

void CarrecPathsWindow::selectVehiclePopup(const char* namePopup, bool* open,
                                           const std::function<void(int)>& callback) {
	const float spacing = ImGui::GetStyle().ItemSpacing.x;
	const float buttonWidth = ImGui::GetFrameHeight();

	if (ImGui::BeginPopupModal(namePopup, open, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::Text("%s", _("carrec_paths.select_vehicle").c_str());
		ImGui::Spacing();

		auto& vehiclesModels = ModelsManager::getVehicleModels();
		if (ImGui::BeginCombo("##regularModelCombo", std::to_string(m_selectedVehicleModel).c_str())) {
			for (auto& item : vehiclesModels) {
				if (ImGui::Selectable(std::to_string(item).c_str(), item == m_selectedVehicleModel)) {
					m_selectedVehicleModel = item;
				}
			}
			ImGui::EndCombo();
		}
		ImGui::SameLine(0.0f, spacing);
		if (ImGui::Button(ICON_FA_CAR, ImVec2(buttonWidth, 0))) {
			PopupVehicleSelector::showPopup();
		}
		std::function<void(int)> vehicleSelectorCallback = [this](int model) { this->m_selectedVehicleModel = model; };
		PopupVehicleSelector::renderPopup(vehicleSelectorCallback);

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::BeginDisabled(m_selectedVehicleModel == -1);
		if (ImGui::Button(_("ok").c_str(), ImVec2(120 * (SCL_PX).x, 0))) {
			callback(m_selectedVehicleModel);
			ImGui::CloseCurrentPopup();
			*open = false;
		}
		ImGui::EndDisabled();

		ImGui::SameLine();
		if (ImGui::Button(_("cancel").c_str(), ImVec2(120 * (SCL_PX).x, 0))) {
			ImGui::CloseCurrentPopup();
			*open = false;
		}

		ImGui::EndPopup();
	}
}

CarrecPathsWindow::CarrecPathsWindow()
	: Window(_("carrec_paths.window_title", ICON_FA_ARROW_PROGRESS)) {
	setPosition(640, 360);
	setPivot(0.5, 0.5);
	setSize(800, 500);
	setFlags(ImGuiWindowFlags_None);
	setRenderCallback<CarrecPathsWindow>(CarrecPathsWindow::renderContent);
}
