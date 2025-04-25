#include "CarrecPathsWindow.h"

#include <CModelInfo.h>
#include <fmt/core.h>

#include <utility>

#include <vector>
#include "CarrecPathsService.h"
#include "EditByPlayerService.h"
#include "fa.h"
#include "imgui.h"
#include "imgui_stdlib.h"
#include "ProjectsService.h"
#include "Settings.h"
#include "utilsRender.h"
#include "VehiclesWindow.h"
#include "WindowsRenderService.h"
#include "Localization/Localization.h"

void Windows::CarrecPathsWindow::selectVehiclePopup(const char *namePopup, bool *open,
                                                    const std::function<void(Vehicle *)> &callback) {
	static auto &local = Localization::getInstance();
	const auto scaleFont = ImGui::GetFontSize() / 16.f;

	if (ImGui::BeginPopupModal(namePopup, open, ImGuiWindowFlags_AlwaysAutoResize)) {
		// vehicle combo
		const auto &vehicles = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getVehicles();
		const auto vehicleIndex = utils::indexByUuid(vehicles, this->vehicleUuid);

		IncorrectHighlight(vehicleIndex == -1, [&] {
			const auto previewValue = vehicleIndex == -1 ? "" : vehicles.at(vehicleIndex)->getName();
			if (ImGui::BeginCombo(local.get("entities.vehicle").c_str(), previewValue.c_str())) {
				for (const auto &vehicle : vehicles) {
					if (ImGui::Selectable(vehicle->getName().c_str(),
					                      this->vehicleUuid == vehicle->getUuid())) {
						this->vehicleUuid = vehicle->getUuid();
					}
				}
				ImGui::EndCombo();
			}
		});

		if (Settings::getInstance().get<bool>("main.autoBindRequireFields").value_or(true)) {
			if (vehicleIndex == -1) {
				if (!vehicles.empty()) {
					this->vehicleUuid = (--vehicles.end())->get()->getUuid();
				}
			}
		}

		ImGui::BeginDisabled(vehicleIndex == -1);
		if (ImGui::Button(local.get("general.ok").c_str(), ImVec2(scaleFont * 100.0f, .0f))) {
			callback(vehicles[vehicleIndex].get());
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndDisabled();

		ImGui::EndPopup();
	}
}

void Windows::CarrecPathsWindow::draw() {
	static auto &local = Localization::getInstance();
	static bool showPopNewRecSelVeh = false;
	static bool showExPopSecVeh = false;
	static std::set<int> useInRecord;
	static std::set<int> useAIRerecord;

	const auto scaleFont = ImGui::GetFontSize() / 16.f;

	const ImVec2 displaySize = ImGui::GetIO().DisplaySize;
	ImGui::SetNextWindowPos(ImVec2(displaySize.x / 2.f, displaySize.y / 2.f), ImGuiCond_Appearing, ImVec2(.5f, .5f));
	if (ImGui::Begin(fmt::format("{} {}", ICON_FA_TRAFFIC_CONE, local.get("carrec_path.title")).c_str(), nullptr,
	                 ImGuiWindowFlags_AlwaysAutoResize)) {
		auto &carrecPaths = CarrecPathsService::getInstance().getPaths();

		ImGui::BeginTable("carrec_paths_table", 4, ImGuiTableFlags_BordersOuter,
		                  ImVec2(0
		                         , scaleFont * 400.f));
		ImGui::TableSetupColumn(local.get("carrec_path.name").c_str());
		ImGui::TableSetupColumn(local.get("entities.vehicle").c_str());
		ImGui::TableSetupColumn(local.get("carrec_path.preview").c_str());
		ImGui::TableSetupColumn(local.get("carrec_path.ai_rerecord").c_str());
		ImGui::TableHeadersRow();

		for (int i = 0; i < static_cast<int>(carrecPaths.size()); ++i) {
			auto &carrecPath = carrecPaths[i];
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			if (ImGui::Selectable(fmt::format("{}##{}", carrecPaths[i].getName(), i).c_str(),
			                      this->selectedPath == i)) {
				this->selectedPath = i;
				useAIRerecord.erase(i);
				useInRecord.erase(i);
			}
			auto openRenamePopup = false;
			if (ImGui::BeginPopupContextItem(fmt::format("##carrec_path_{}", i).c_str())) {
				if (ImGui::MenuItem(local.get("general.rename").c_str())) {
					this->selectedPath = i;
					openRenamePopup = true;
				}
				if (ImGui::MenuItem(local.get("general.delete").c_str())) {
					carrecPaths.erase(carrecPaths.begin() + i);
					this->selectedPath = std::clamp(this->selectedPath, -1, static_cast<int>(carrecPaths.size()) - 1);
				}
				ImGui::EndPopup();
			}

			ImGui::PushID(i);
			if (openRenamePopup) {
				ImGui::OpenPopup("##renameItem");
			}

			if (ImGui::BeginPopup("##renameItem")) {
				ImGui::InputText("##renameItemInput", &carrecPath.getName());
				ImGui::EndPopup();
			}
			ImGui::PopID();

			ImGui::TableSetColumnIndex(1);
			const auto name = &reinterpret_cast<CVehicleModelInfo*>(CModelInfo::GetModelInfo(
				carrecPath.getVehicle().getModelId()))->m_szGameName[0];
			if (ImGui::Button(name, ImVec2(scaleFont * 100.0f, .0f))) {
				ImGui::OpenPopup("##existsSelectVehicle");
				showExPopSecVeh = true;
			}

			selectVehiclePopup("##existsSelectVehicle", &showExPopSecVeh, [&carrecPath](const Vehicle *vehicle) {
				carrecPath.getVehicle() = vehicle->copy();
				carrecPath.getVehicle().getName() = "";
			});

			ImGui::TableSetColumnIndex(2);
			auto previewToggle = useInRecord.contains(i);
			if (ImGui::Checkbox(fmt::format("##previewRec{}", i).c_str(), &previewToggle)) {
				if (previewToggle) {
					useInRecord.insert(i);
					if (selectedPath == i) {
						selectedPath = -1;
					}
				} else {
					useInRecord.erase(i);
					useAIRerecord.erase(i);
				}
			}

			ImGui::TableSetColumnIndex(3);
			auto rerecordToggle = useAIRerecord.contains(i);
			if (ImGui::Checkbox(fmt::format("##rerecord{}", i).c_str(), &rerecordToggle)) {
				if (rerecordToggle) {
					useAIRerecord.insert(i);
					useInRecord.insert(i);
					if (selectedPath == i) {
						selectedPath = -1;
					}
				} else {
					useAIRerecord.erase(i);
				}
			}
		}
		ImGui::EndTable();

		if (ImGui::Button(local.get("carrec_path.rec").c_str(), ImVec2(scaleFont * 100.0f, .0f))) {
			ImGui::OpenPopup("##newRecSelectVehicle");
			showPopNewRecSelVeh = true;
		}

		selectVehiclePopup("##newRecSelectVehicle", &showPopNewRecSelVeh, [&](const Vehicle *vehicle) {
			const auto newPath = CarrecPathsService::getInstance().createNewPath();
			newPath->getVehicle() = vehicle->copy();
			newPath->getVehicle().getName() = newPath->getName();

			std::vector<CarrecPath*> previewPaths;
			std::vector<bool> aiRerecord;
			for (const auto &i : useInRecord) {
				previewPaths.push_back(&carrecPaths[i]);
				aiRerecord.push_back(useAIRerecord.contains(i));
			}

			const CallbackCarrecPath callback = [newPath, &carrecPaths, previewPaths](const bool isRecorded,
				std::vector<CVehicleStateEachFrame>
				vehicleStateEachFrames,
				const std::map<
					int, std::vector<CVehicleStateEachFrame>> &
				rerecordsPaths) {
				if (isRecorded) {
					newPath->getPath() = std::move(vehicleStateEachFrames);
					for (const auto &[index, path] : rerecordsPaths) {
						previewPaths[index]->getPath() = path;
					}
				}
			};

			EditByPlayerService::getInstance().editByPlayerCarrecPath(
				0, *newPath, previewPaths, aiRerecord, callback);
		});

		ImGui::SameLine();
		ImGui::BeginDisabled(this->selectedPath == -1);
		const auto playTextWidth = ImGui::CalcTextSize(local.get("carrec_path.play").c_str()).x;
		if (ImGui::Button(local.get("carrec_path.play").c_str(), ImVec2(playTextWidth + scaleFont * 10, .0f))) {
			auto &carrecPath = carrecPaths[this->selectedPath];

			std::vector<CarrecPath*> previewPaths;
			std::vector<bool> aiRerecord;
			for (const auto &i : useInRecord) {
				previewPaths.push_back(&carrecPaths[i]);
				aiRerecord.push_back(false);
			}

			auto callback = [&](const bool,
			                    const std::vector<CVehicleStateEachFrame> &,
			                    const std::map<int, std::vector<CVehicleStateEachFrame>> &) {};
			EditByPlayerService::getInstance().
				editByPlayerCarrecPath(1, carrecPath, previewPaths, aiRerecord, callback);
		}
		ImGui::SameLine();
		const auto editTextWidth = ImGui::CalcTextSize(local.get("carrec_path.edit").c_str()).x;
		if (ImGui::Button(local.get("carrec_path.rerec").c_str(), ImVec2(editTextWidth + scaleFont * 10, .0f))) {
			auto &carrecPath = carrecPaths[this->selectedPath];

			std::vector<CarrecPath*> previewPaths;
			std::vector<bool> aiRerecord;
			for (const auto &i : useInRecord) {
				previewPaths.push_back(&carrecPaths[i]);
				aiRerecord.push_back(useAIRerecord.contains(i));
			}

			auto callback = [&carrecPath, previewPaths](const bool isRecorded,
			                                            std::vector<CVehicleStateEachFrame> vehicleStateEachFrames,
			                                            const std::map<int, std::vector<CVehicleStateEachFrame>> &
			                                            rerecordsPaths) {
				if (isRecorded) {
					carrecPath.getPath() = std::move(vehicleStateEachFrames);
					for (const auto &[index, path] : rerecordsPaths) {
						previewPaths[index]->getPath() = path;
					}
				}
			};

			EditByPlayerService::getInstance().editByPlayerCarrecPath(0, carrecPath, previewPaths, aiRerecord,
			                                                          callback);
		}
		ImGui::SameLine();
		const auto createCopyTextWidth = ImGui::CalcTextSize(local.get("carrec_path.create_copy_vehicle").c_str()).x;
		if (ImGui::Button(local.get("carrec_path.create_copy_vehicle").c_str(),
		                  ImVec2(createCopyTextWidth + scaleFont * 10, .0f))) {
			auto &carrecPath = carrecPaths[this->selectedPath];
			ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewVehicleFrom(
				carrecPath.getVehicle());
			auto &vehicle = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getVehicles().back();
			vehicle->spawnEditorVehicle();
			WindowsRenderService::getInstance().replaceWindow<CarrecPathsWindow, VehiclesWindow>();
		}

		ImGui::EndDisabled();
	}
	ImGui::End();
}
