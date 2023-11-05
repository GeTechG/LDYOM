#include "CheckpointsWindow.h"

#include <CCamera.h>
#include <extensions/ScriptCommands.h>


#include "fa.h"
#include "imgui.h"
#include "utils.h"
#include "utilsRender.h"
#include "fmt/core.h"

std::string Windows::CheckpointsWindow::getNameList() {
	return fmt::format("{} {}", ICON_FA_MAP_MARKER_ALT, Localization::getInstance().get("entities.checkpoints"));
}

std::string Windows::CheckpointsWindow::getNameOption() {
	return fmt::format("{} {}", ICON_FA_MAP_MARKER_ALT, Localization::getInstance().get("entities.checkpoint"));
}

int Windows::CheckpointsWindow::getListSize() {
	return static_cast<int>(ProjectsService::getInstance()
	                        .getCurrentProject()
	                        .getCurrentScene()
	                        ->getCheckpoints()
	                        .size());
}

void Windows::CheckpointsWindow::createNewElement() {
	ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewCheckpoint();
}

void Windows::CheckpointsWindow::createNewElementFrom(int i) {
	const auto &checkpoint = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getCheckpoints().
	                                                        at(i);
	ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewCheckpointFrom(*checkpoint);
	ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getCheckpoints().back()->
	                               spawnEditorCheckpoint();
}

char* Windows::CheckpointsWindow::getElementName(int i) {
	return ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getCheckpoints().at(i)->getName();
}

void Windows::CheckpointsWindow::deleteElement(int i) {
	ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getCheckpoints().at(i)->
	                               deleteEditorCheckpoint();

	const auto begin = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getCheckpoints().begin();
	ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getCheckpoints().erase(begin + i);
	this->currentElement--;
}

void Windows::CheckpointsWindow::drawOptions() {
	auto &local = Localization::getInstance();

	Checkpoint *checkpoint = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getCheckpoints().at(
		this->currentElement).get();

	//position
	DragPosition(checkpoint->getPosition(), [checkpoint] {
		checkpoint->updateLocation();
	});

	ImGui::Separator();

	if (ImGui::SliderInt(local.get("general.type").c_str(), &checkpoint->getType(), 0, 1,
	                     local.getArray("checkpoint.types")[checkpoint->getType()].c_str())) {
		checkpoint->getCheckpointType() = 0;
		checkpoint->spawnEditorCheckpoint();
	}

	bool changed = false;

	if (checkpoint->getType() == 0) {
		changed |= ImGui::SliderInt(local.get("checkpoint.type_sphere").c_str(), &checkpoint->getCheckpointType(), 0,
		                            6);

		changed |= ImGui::ColorEdit4(local.get("checkpoint.color").c_str(), checkpoint->getColor().data());
		changed |= ImGui::DragInt(local.get("checkpoint_objective.pulse_period").c_str(), &checkpoint->getPulsePeriod(),
		                          64, 0, USHRT_MAX);
		changed |= ImGui::InputFloat(local.get("checkpoint_objective.pulse_fraction").c_str(),
		                             &checkpoint->getPulseFraction(), .0f, .0f);
		changed |= ImGui::DragInt(local.get("checkpoint_objective.rotate_rate").c_str(), &checkpoint->getRotateRate(),
		                          10, SHRT_MIN, SHRT_MAX);
		changed |= ImGui::DragFloat(local.get("general.radius").c_str(), &checkpoint->getRadius(), 0.05f, 0.0f, 100.0f);
	} else {
		if (ImGui::SliderInt(local.get("checkpoint.type_sphere").c_str(), &checkpoint->getCheckpointType(), 0, 8))
			checkpoint->spawnEditorCheckpoint();
		changed |= ImGui::DragFloat(local.get("general.rotate").c_str(), &checkpoint->getAngle(), 0.1f, -180.f, 180.f,
		                            "%.2f°");
		if (ImGui::DragFloat(local.get("general.radius").c_str(), &checkpoint->getRadius(), 0.05f, 0.0f, 100.0f)) {
			checkpoint->spawnEditorCheckpoint();
		}
	}

	if (changed)
		checkpoint->updateLocation();

	ImGui::Separator();

	if (ImGui::SliderInt(local.get("general.type_marker").c_str(), &checkpoint->getBlipType(), 0, 1,
	                     local.getArray("general.type_marker_enum").at(checkpoint->getBlipType()).c_str()))
		checkpoint->spawnEditorBlip();

	if (checkpoint->getBlipType() == 0) {
		if (utils::Combo(local.get("general.color_marker").c_str(), &checkpoint->getBlipColor(),
		                 local.getArray("general.color_marker_enum"), 9)) {
			checkpoint->spawnEditorBlip();
		}
	} else {
		popupSpriteBlipSelector_.draw(&checkpoint->getBlipSprite(), [checkpoint](int i) {
			checkpoint->spawnEditorCheckpoint();
		});
	}

	ObjectiveDependentInput(checkpoint);

	this->listOverlays.emplace_back(local.get("info_overlay.camera_view"));
	this->listOverlays.emplace_back(local.get("info_overlay.depend_zoom"));
	this->listOverlays.emplace_back(local.get("info_overlay.move_element"));

	if (utils::controlCameraWithMove(checkpoint->getPosition())) {
		checkpoint->updateLocation();
		checkpoint->spawnEditorBlip();
	}
}

void Windows::CheckpointsWindow::close() {
	ListWindow::close();
	TheCamera.Restore();
	plugin::Command<plugin::Commands::SET_PLAYER_CONTROL>(0, true);
	plugin::Command<plugin::Commands::SET_CHAR_PROOFS>(static_cast<CPed*>(FindPlayerPed()), 1, 1, 1, 1, 1);
}

void Windows::CheckpointsWindow::open() {
	ListWindow::open();
	plugin::Command<plugin::Commands::SET_PLAYER_CONTROL>(0, false);
}
