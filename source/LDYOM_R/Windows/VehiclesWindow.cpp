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
	if (ImGui::TreeNode(local.get("general.characteristics").c_str())) {
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
						                                    ImGuiColorEditFlags_AlphaBar + ImGuiColorEditFlags_NoInputs
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

		ImGui::Separator();

		ImGui::PushItemWidth(150.0f);
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

		ImGui::PopItemWidth();

		ImGui::Separator();

		const auto &vehicleUnbreakable = local.getArray("vehicle.vehicle_unbreakable");
		utils::ToggleButton(vehicleUnbreakable[0].c_str(), &vehicle->isBulletproof());
		utils::ToggleButton(vehicleUnbreakable[1].c_str(), &vehicle->isFireproof());
		utils::ToggleButton(vehicleUnbreakable[2].c_str(), &vehicle->isExplosionproof());
		utils::ToggleButton(vehicleUnbreakable[3].c_str(), &vehicle->isCollisionproof());
		utils::ToggleButton(vehicleUnbreakable[4].c_str(), &vehicle->isMeleeproof());
		utils::ToggleButton(vehicleUnbreakable[5].c_str(), &vehicle->isTiresVulnerability());

		ImGui::TreePop();
	}

	utils::ToggleButton(local.get("vehicle.locked").c_str(), &vehicle->isLocked());
	utils::ToggleButton(local.get("general.must_survive").c_str(), &vehicle->isShouldNotDie());
}

void Windows::VehiclesWindow::drawOptions() {
	auto &local = Localization::getInstance();

	Vehicle *vehicle = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getVehicles().at(
		this->currentElement).get();

	//position
	InputPosition(vehicle->getPosition(), [vehicle] { vehicle->updateLocation(); });
	//heading
	DragAngleRotation(&vehicle->getHeadingAngle(), [vehicle] { vehicle->updateLocation(); });

	//modelSelection(vehicle, local);

	if (ImGui::Button(local.get("general.model").c_str())) {
		popupVehicleSelector_.show();
	}

	characteristicsSection(local, vehicle);

	popupVehicleSelector_.draw([&](const int model) {
		vehicle->getModelId() = model;
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
