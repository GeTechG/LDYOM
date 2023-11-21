#include "TrainsWindow.h"

#include <CCamera.h>
#include <extensions/ScriptCommands.h>

#include "fa.h"
#include "imgui.h"
#include "utils.h"
#include "utilsRender.h"
#include "fmt/core.h"

std::string Windows::TrainsWindow::getNameList() {
	return fmt::format("{} {}", ICON_FA_TRAIN, Localization::getInstance().get("entities.trains"));
}

std::string Windows::TrainsWindow::getNameOption() {
	return fmt::format("{} {}", ICON_FA_TRAIN, Localization::getInstance().get("entities.train"));
}

int Windows::TrainsWindow::getListSize() {
	return static_cast<int>(ProjectsService::getInstance()
	                        .getCurrentProject()
	                        .getCurrentScene()
	                        ->getTrains()
	                        .size());
}

void Windows::TrainsWindow::createNewElement() {
	ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewTrain();
}

void Windows::TrainsWindow::createNewElementFrom(int i) {
	const auto &train = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getTrains().at(i);
	ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewTrainFrom(*train);
	ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getTrains().back()->spawnEditorTrain();
}

std::string& Windows::TrainsWindow::getElementName(int i) {
	return ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getTrains().at(i)->getName();
}

void Windows::TrainsWindow::deleteElement(int i) {
	ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getTrains().at(i)->deleteEditorTrain();

	const auto begin = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getTrains().begin();
	ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getTrains().erase(begin + i);
	this->currentElement--;
}

void characteristicsSection(Localization &local, Train *train) {
	if (ImGui::TreeNode(local.get("general.characteristics").c_str())) {
		ImGui::PushItemWidth(150.0f);
		ImGui::InputInt(local.get("general.health").c_str(), &train->getHealth(), 0, 0);

		if (utils::ToggleButton(local.get("general.rotate").c_str(), &train->isRotate()) ||
			ImGui::SliderInt(local.get("general.type").c_str(), &train->getTrainType(), 0, 15))
			train->spawnEditorTrain();

		ImGui::InputFloat(local.get("train.speed").c_str(), &train->getSpeed());
		ImGui::InputFloat(local.get("train.cruise_speed").c_str(), &train->getCruiseSpeed());

		ImGui::TreePop();
	}
}

void Windows::TrainsWindow::drawOptions() {
	auto &local = Localization::getInstance();

	Train *train = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getTrains().at(
		this->currentElement).get();

	//position
	InputPosition(train->getPosition(), [train] { train->updateLocation(); });

	//modelSelection(vehicle, local);

	characteristicsSection(local, train);

	ObjectiveDependentInput(train);

	this->listOverlays.emplace_back(local.get("info_overlay.camera_view"));

	utils::controlCamera(CVector(train->getPosition()[0], train->getPosition()[1], train->getPosition()[2] + 1.0f));
}

void Windows::TrainsWindow::close() {
	ListWindow::close();
	TheCamera.Restore();
	plugin::Command<plugin::Commands::SET_PLAYER_CONTROL>(0, true);
	plugin::Command<plugin::Commands::SET_CHAR_PROOFS>(static_cast<CPed*>(FindPlayerPed()), 1, 1, 1, 1, 1);
}

void Windows::TrainsWindow::open() {
	ListWindow::open();
	plugin::Command<plugin::Commands::SET_PLAYER_CONTROL>(0, false);
}
