#include "PickupWindow.h"

#include <CCamera.h>
#include <imHotKey.h>
#include <extensions/ScriptCommands.h>

#include "fa.h"
#include "imgui.h"
#include "PopupWeaponSelector.h"
#include "utils.h"
#include "utilsRender.h"
#include "fmt/core.h"

std::string Windows::PickupWindow::getNameList() {
	return fmt::format("{} {}", ICON_FA_HEART, Localization::getInstance().get("entities.pickups"));
}

std::string Windows::PickupWindow::getNameOption() {
	return fmt::format("{} {}", ICON_FA_HEART, Localization::getInstance().get("entities.pickup"));
}

int Windows::PickupWindow::getListSize() {
	return static_cast<int>(ProjectsService::getInstance()
		.getCurrentProject()
		.getCurrentScene()
		->getPickups()
		.size());
}

void Windows::PickupWindow::createNewElement() {
	ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewPickup();
}

char* Windows::PickupWindow::getElementName(int i) {
	return ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getPickups().at(i)->getName();
}

void Windows::PickupWindow::deleteElement(int i) {
	ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getPickups().at(i)->deleteEditorPickup();

	const auto begin = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getPickups().begin();
	ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getPickups().erase(begin + i);
	this->currentElement--;
}

void Windows::PickupWindow::drawOptions() {
	auto& local = Localization::getInstance();

	Pickup* pickup = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getPickups().at(this->currentElement).get();

	//position
	DragPosition(pickup->getPosition(), [pickup] {
		pickup->updateLocation();
		});

	ImGui::Separator();

	ImGui::SliderInt(local.get("pickup.spawn_type").c_str(), &pickup->getSpawnType(), 0, 2,local.getArray("pickup.spawn_types_pickup")[pickup->getSpawnType()].c_str());
	if (utils::Combo(local.get("general.type").c_str(), &pickup->getType(), local.getArray("pickup.pickup_types"))) {
		pickup->spawnEditorPickup();
	}

	if (pickup->getType() == 0) {
		ImGui::Separator();
		PopupWeaponSelector::getInstance().weaponButton(&pickup->getWeapon());

		PopupWeaponSelector::getInstance().renderPopup([pickup](const int weaponId) {
			pickup->getWeapon() = weaponId;
			pickup->spawnEditorPickup();
		});

		ImGui::InputInt(local.get("weapons.ammo").c_str(), &pickup->getAmmo(), 0, 0);
	} else if (pickup->getType() == 5) {
		if (ImGui::Button(ICON_FA_CUBES, ImVec2(ImGui::GetFontSize() * 1.56f, ImGui::GetFontSize() * 1.56f))) {
			objectSelectorPopup_.open();
			objectSelectorPopup_.setCallbackSelect([pickup](int modelId) {
				pickup->getModelId() = modelId;
				pickup->spawnEditorPickup();
				});
		}
		if (ImGui::IsItemHovered()) {
			ImGui::SetTooltip(local.get("object_selector.title").c_str());
		}
		ImGui::SameLine();
		ImGui::InputInt(local.get("general.model").c_str(), &pickup->getModelId());
		if (ImGui::Button(local.get("general.apply").c_str())) {
			pickup->spawnEditorPickup();
		}
		this->objectSelectorPopup_.draw();
	}

	ObjectiveDependentInput(pickup);

	constexpr ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
	if (ImGui::Begin("##controlOverlay", nullptr, windowFlags)) {
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 16.5f);
		ImGui::Text(local.get("info_overlay.camera_view").c_str());
		ImGui::Text(local.get("info_overlay.depend_zoom").c_str());
		ImGui::Text(local.get("info_overlay.move_element").c_str());
	}
	ImGui::End();

	if (utils::controlCameraWithMove(pickup->getPosition())) {
		pickup->updateLocation();
	}
}

void Windows::PickupWindow::close() {
	ListWindow::close();
	TheCamera.Restore();
	plugin::Command<plugin::Commands::SET_PLAYER_CONTROL>(0, true);
}

void Windows::PickupWindow::open() {
	ListWindow::open();
	plugin::Command<plugin::Commands::SET_PLAYER_CONTROL>(0, false);
}