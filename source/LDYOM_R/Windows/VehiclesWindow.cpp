#include "VehiclesWindow.h"

#include <array>
#include <CCamera.h>
#include <extensions/ScriptCommands.h>

#include "components.h"
#include "EditByPlayerService.h"
#include "fa.h"
#include "imgui.h"
#include "imgui_stdlib.h"
#include "ProjectsService.h"
#include "utils.h"
#include "utilsRender.h"
#include "fmt/core.h"
#include "Localization/Localization.h"

std::string Windows::VehiclesWindow::getNameList() {
	return fmt::format("{} {}", ICON_FA_CARS, Localization::getInstance().get("entities.vehicles"));
}

std::string Windows::VehiclesWindow::getNameOption() {
	return fmt::format("{} {}", ICON_FA_CAR, Localization::getInstance().get("entities.vehicle"));
}

int Windows::VehiclesWindow::getListSize() {
	return static_cast<int>(ProjectsService::getInstance()
	                        .getCurrentProject()
	                        .getCurrentScene()
	                        ->getVehicles()
	                        .size());
}

void Windows::VehiclesWindow::createNewElement() {
	ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewVehicle();
}

void Windows::VehiclesWindow::createNewElementFrom(int i) {
	const auto &vehicle = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getVehicles().at(i);
	ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewVehicleFrom(*vehicle);
	ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getVehicles().back()->spawnEditorVehicle();
}

std::string& Windows::VehiclesWindow::getElementName(int i) {
	return ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getVehicles().at(i)->getName();
}

void Windows::VehiclesWindow::deleteElement(int i) {
	ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getVehicles().at(i)->deleteEditorVehicle();

	const auto begin = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getVehicles().begin();
	ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getVehicles().erase(begin + i);
	this->currentElement--;
}

void characteristicsSection(Localization &local, Vehicle *vehicle) {
	const auto scaleFont = ImGui::GetFontSize() / 16.f;
	if (ImGui::TreeNode(local.get("general.characteristics").c_str())) {
		ImGui::BeginChild("##characteristics", ImVec2(0, scaleFont * 200.f));
		if (!vehicle->isRecolorBanned()) {
			if (utils::ToggleButton(local.get("vehicle.game_colors").c_str(), &vehicle->isIsGameColorsMode())) {
				if (vehicle->getEditorVehicle().has_value()) {
					vehicle->spawnEditorVehicle(true);
				}
			}
		}
		static unsigned char *colorId = nullptr;
		if (vehicle->isIsGameColorsMode() || vehicle->isRecolorBanned()) {
			const auto primary = CVehicleModelInfo::ms_vehicleColourTable[vehicle->getPrimaryColorId()];
			if (ImGui::ColorButton("##primaryColor",
			                       ImColor(primary.r, primary.g, primary.b),
			                       ImGuiColorEditFlags_NoTooltip)) {
				colorId = &vehicle->getPrimaryColorId();
				ImGui::OpenPopup("colorPopupSelect");
			}
			ImGui::SameLine();
			ImGui::Text(local.get("vehicle.primary_color_game").c_str());
			const auto secondary = CVehicleModelInfo::ms_vehicleColourTable[vehicle->getSecondaryColorId()];
			if (ImGui::ColorButton("##secondaryColor",
			                       ImColor(secondary.r, secondary.g, secondary.b),
			                       ImGuiColorEditFlags_NoTooltip)) {
				colorId = &vehicle->getSecondaryColorId();
				ImGui::OpenPopup("colorPopupSelect");
			}
			ImGui::SameLine();
			ImGui::Text(local.get("vehicle.secondary_color_game").c_str());
			const auto tertiary = CVehicleModelInfo::ms_vehicleColourTable[vehicle->getTertiaryColorId()];
			if (ImGui::ColorButton("##tertiaryColor",
			                       ImColor(tertiary.r, tertiary.g, tertiary.b),
			                       ImGuiColorEditFlags_NoTooltip)) {
				colorId = &vehicle->getTertiaryColorId();
				ImGui::OpenPopup("colorPopupSelect");
			}
			ImGui::SameLine();
			ImGui::Text(local.get("vehicle.tertiary_color_game").c_str());
			const auto quaternary = CVehicleModelInfo::ms_vehicleColourTable[vehicle->getQuaternaryColorId()];
			if (ImGui::ColorButton("##quaternaryColor",
			                       ImColor(quaternary.r, quaternary.g, quaternary.b, quaternary.a),
			                       ImGuiColorEditFlags_NoTooltip)) {
				colorId = &vehicle->getQuaternaryColorId();
				ImGui::OpenPopup("colorPopupSelect");
			}
			ImGui::SameLine();
			ImGui::Text(local.get("vehicle.quaternary_color_game").c_str());
		} else {
			utils::ToggleButton(local.get("vehicle.extended_colors").c_str(), &vehicle->isExtendedColor());
			if (vehicle->isExtendedColor()) {
				if (ImGui::TreeNode(local.get("general.colors").c_str())) {
					components::extractComObjMatVehicle(vehicle->getEditorVehicle().value(),
					                                    [&](int i, components::VehicleComponent comp,
					                                        components::VehicleAtomic obj,
					                                        components::VehicleMaterial mat) {
						                                    ImGui::PushID(i);
						                                    if (ImGui::ColorEdit4(
							                                    std::to_string(i).c_str(),
							                                    vehicle->getColors()[i].second.data(),
							                                    ImGuiColorEditFlags_AlphaBar +
							                                    ImGuiColorEditFlags_NoInputs
							                                    +
							                                    ImGuiColorEditFlags_NoLabel)) {
							                                    mat.setColor(
								                                    floatColorToCRGBA(vehicle->getColors()[i].second));
						                                    }
						                                    ImGui::SameLine();
						                                    const auto &nameC = fmt::format(
							                                    "{}:{}:{}", comp.getName(), !mat._material->texture
									                                    ? "[no texture]"
									                                    : mat._material->texture->name,
							                                    mat.getRawPointer());
						                                    ImGui::Text(nameC.c_str());
						                                    ImGui::PopID();
					                                    });
					ImGui::TreePop();
				}
			} else {
				if (ImGui::ColorEdit4("##primaryColorE", vehicle->getPrimaryColor(),
				                      ImGuiColorEditFlags_AlphaBar + ImGuiColorEditFlags_NoInputs +
				                      ImGuiColorEditFlags_NoLabel)) {
					vehicle->setEditorPrimaryColor();
				}
				ImGui::SameLine();
				ImGui::Text(local.get("vehicle.primary_color").c_str());

				if (ImGui::ColorEdit4("##secondaryColorE", vehicle->getSecondaryColor(),
				                      ImGuiColorEditFlags_AlphaBar + ImGuiColorEditFlags_NoInputs +
				                      ImGuiColorEditFlags_NoLabel)) {
					vehicle->setEditorSecondaryColor();
				}
				ImGui::SameLine();
				ImGui::Text(local.get("vehicle.secondary_color").c_str());
			}
		}

		auto applyColors = [vehicle] {
			if (vehicle->getEditorVehicle().has_value()) {
				auto a = vehicle->getEditorVehicle().value()->m_nPrimaryColor;
				vehicle->getEditorVehicle().value()->m_nPrimaryColor = vehicle->getPrimaryColorId();
				a = vehicle->getEditorVehicle().value()->m_nPrimaryColor;
				vehicle->getEditorVehicle().value()->m_nSecondaryColor = vehicle->getSecondaryColorId();
				vehicle->getEditorVehicle().value()->m_nTertiaryColor = vehicle->getTertiaryColorId();
				vehicle->getEditorVehicle().value()->m_nQuaternaryColor = vehicle->getQuaternaryColorId();
			}
		};

		if (ImGui::BeginPopup("colorPopupSelect")) {
			for (int i = 0; i < 128; ++i) {
				const auto crgba = CVehicleModelInfo::ms_vehicleColourTable[i];
				if (i % 8 != 0) {
					ImGui::SameLine();
				}
				if (ImGui::ColorButton(std::to_string(i).c_str(), ImColor(crgba.r, crgba.g, crgba.b, crgba.a),
				                       ImGuiColorEditFlags_NoTooltip)) {
					*colorId = static_cast<unsigned char>(i);
					applyColors();
					ImGui::CloseCurrentPopup();
				}
			}
			ImGui::EndPopup();
		}

		ImGui::Separator();

		ImGui::PushItemWidth(scaleFont * 150.0f);
		ImGui::InputInt(local.get("general.health").c_str(), &vehicle->getHealth(), 0, 0);

		if (ImGui::SliderInt(fmt::format("{} A", local.get("vehicle.component")).c_str(), &vehicle->getComponentTypeA(),
		                     -1, 5)) {
			vehicle->spawnEditorVehicle(true);
		}
		if (ImGui::SliderInt(fmt::format("{} B", local.get("vehicle.component")).c_str(), &vehicle->getComponentTypeB(),
		                     -1, 5)) {
			vehicle->spawnEditorVehicle(true);
		}
		if (ImGui::InputText(local.get("vehicle.numberplate").c_str(), &vehicle->getNumberplate()))
			vehicle->spawnEditorVehicle();

		static const std::string cities_names[3] = {"Los Santos", "San Fierro", "Las Veturas"};

		if (ImGui::SliderInt(local.get("general.city").c_str(), &vehicle->getNumberplateCity(), -1, 1,
		                     cities_names[vehicle->getNumberplateCity() + 1].c_str()))
			vehicle->spawnEditorVehicle();

		if (ImGui::SliderAngle(local.get("vehicle.extra_parts_angle").c_str(), &vehicle->getExtraPartsAngle(), 0.f,
		                       360.f)) {
			static_cast<CAutomobile*>(vehicle->getEditorVehicle().value())->UpdateMovingCollision(
				vehicle->getExtraPartsAngle());
		}
		ImGui::SetItemTooltip(local.get("vehicle.extra_parts_angle_info").c_str());

		if (ImGui::Checkbox(local.get("vehicle.lights_on").c_str(), &vehicle->isIsLightsOn())) {
			Command<Commands::FORCE_CAR_LIGHTS>(vehicle->getEditorVehicle().value(),
			                                    vehicle->isIsLightsOn() ? 2 : 0);
			vehicle->getEditorVehicle().value()->m_nVehicleFlags.bEngineOn = static_cast<unsigned char>(vehicle->
				isIsLightsOn());
		}

		if (ImGui::SliderFloat(local.get("vehicle.dirty_level").c_str(), &vehicle->getDirtyLevel(), 0.f, 15.f)) {
			vehicle->getEditorVehicle().value()->m_fDirtLevel = vehicle->getDirtyLevel();
		}

		ImGui::PopItemWidth();

		ImGui::Separator();

		const auto &vehicleUnbreakable = local.getArray("vehicle.vehicle_unbreakable");
		utils::ToggleButton(vehicleUnbreakable[0].c_str(), &vehicle->isBulletproof());
		utils::ToggleButton(vehicleUnbreakable[1].c_str(), &vehicle->isFireproof());
		utils::ToggleButton(vehicleUnbreakable[2].c_str(), &vehicle->isExplosionproof());
		utils::ToggleButton(vehicleUnbreakable[3].c_str(), &vehicle->isCollisionproof());
		utils::ToggleButton(vehicleUnbreakable[4].c_str(), &vehicle->isMeleeproof());
		utils::ToggleButton(vehicleUnbreakable[5].c_str(), &vehicle->isTiresVulnerability());
		if (utils::ToggleButton(local.get("vehicle.heavy").c_str(), &vehicle->isHeavy())) {
			Command<Commands::SET_CAR_HEAVY>(vehicle->getEditorVehicle().value(), vehicle->isHeavy());
		}

		ImGui::EndChild();
		ImGui::TreePop();
	}
}

void damagesComponents(Localization &local, Vehicle *vehicle) {
	if (ImGui::TreeNode(local.get("vehicle.damages").c_str())) {
		const auto scaleFont = ImGui::GetFontSize() / 16.f;
		ImGui::BeginChild("##damagesComponents", ImVec2(0, scaleFont * 200.f));
		ImGui::PushItemWidth(scaleFont * 150.0f);
		for (int i = 0; i < 17; i++) {
			if (ImGui::DragFloat(local.getArray("vehicle.components")[i].c_str(), &vehicle->getDamagesComponents()[i],
			                     1.f, 0, 200.f)) {
				vehicle->spawnEditorVehicle();
			}
		}
		ImGui::PopItemWidth();
		ImGui::EndChild();
		ImGui::TreePop();
	}
}

void openDoors(Localization &local, Vehicle *vehicle) {
	if (ImGui::TreeNode(local.get("vehicle.open_doors").c_str())) {
		const auto scaleFont = ImGui::GetFontSize() / 16.f;
		ImGui::BeginChild("##openDoors", ImVec2(0, scaleFont * 200.f));
		ImGui::PushItemWidth(scaleFont * 150.0f);
		for (int i = 0; i < 6; i++) {
			if (ImGui::SliderFloat(local.getArray("vehicle.components")[i + 4].c_str(),
			                       &vehicle->getOpenDoorsRation()[i],
			                       0.f, 1.f)) {
				const int carNodeIndexFromDoor = utils::GetCarNodeIndexFromDoor(i);
				vehicle->getEditorVehicle().value()->OpenDoor(nullptr, carNodeIndexFromDoor, static_cast<eDoors>(i),
				                                              vehicle->getOpenDoorsRation()[i], false);
			}
		}
		ImGui::PopItemWidth();
		ImGui::EndChild();
		ImGui::TreePop();
	}
}

void Windows::VehiclesWindow::drawOptions() {
	auto &local = Localization::getInstance();

	Vehicle *vehicle = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getVehicles().at(
		this->currentElement).get();

	//position
	DragPosition(vehicle->getPosition(), [vehicle] { vehicle->updateLocation(); });
	//heading
	DragAngleRotation(&vehicle->getHeadingAngle(), [vehicle] { vehicle->updateLocation(); });

	if (ImGui::Button(local.get("general.model").c_str())) {
		popupVehicleSelector_.show();
	}

	characteristicsSection(local, vehicle);
	damagesComponents(local, vehicle);
	openDoors(local, vehicle);

	utils::ToggleButton(local.get("vehicle.locked").c_str(), &vehicle->isLocked());
	utils::ToggleButton(local.get("general.must_survive").c_str(), &vehicle->isShouldNotDie());

	popupVehicleSelector_.draw([&](const int model) {
		vehicle->getModelId() = model;
		vehicle->getPaintjob() = -1;
		vehicle->getUpgrades().fill(-1);
		vehicle->spawnEditorVehicle(true);
	});

	ObjectiveDependentInput(vehicle);

	if (ImGui::Button(local.get("general.edit_manually").c_str())) {
		EditByPlayerService::getInstance().editByPlayerVehicle(*vehicle);
	}

	this->listOverlays.emplace_back(local.get("info_overlay.camera_view"));


	utils::controlCamera(
		CVector(vehicle->getPosition()[0], vehicle->getPosition()[1], vehicle->getPosition()[2] + 1.0f));
}

void Windows::VehiclesWindow::close() {
	ListWindow::close();
	TheCamera.Restore();
	plugin::Command<Commands::SET_PLAYER_CONTROL>(0, true);
	plugin::Command<Commands::SET_CHAR_PROOFS>(static_cast<CPed*>(FindPlayerPed()), 1, 1, 1, 1, 1);
}

void Windows::VehiclesWindow::open() {
	ListWindow::open();
	plugin::Command<Commands::SET_PLAYER_CONTROL>(0, false);
}
