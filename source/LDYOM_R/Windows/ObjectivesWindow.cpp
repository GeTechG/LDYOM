#include "ObjectivesWindow.h"

#include <CCamera.h>
#include <CWorld.h>

#include "fa.h"
#include "Localization/Localization.h"
#include "imgui.h"
#include "../Data/CheckpointObjective.h"
#include "fmt/core.h"

class CheckpointObjective;

std::string Windows::ObjectivesWindow::getNameList() {
	return fmt::format("{} {}", ICON_FA_TASKS, Localization::getInstance().get("objective.objectives"));
}

std::string Windows::ObjectivesWindow::getNameOption() {
	return Localization::getInstance().get("objective.title");
}

int Windows::ObjectivesWindow::getListSize() {
	return static_cast<int>(ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getObjectives().size());
}

void Windows::ObjectivesWindow::createNewElement() {}

char* Windows::ObjectivesWindow::getElementName(int i) {
	return ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getObjectives().at(i)->getName();
}

void Windows::ObjectivesWindow::deleteElement(int i) {
	const auto begin = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getObjectives().begin();
	ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getObjectives().erase(begin + i);
	this->currentElement--;
}

void Windows::ObjectivesWindow::drawOptions() {
	auto& local = Localization::getInstance();

	BaseObjective* objective = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getObjectives().at(this->currentElement).get();

	objective->draw(local);
}

void Windows::ObjectivesWindow::onButtonCreateNewElement() {
	ImGui::OpenPopup("##modalObjectiveSelector");
}

void Windows::ObjectivesWindow::drawListWindow() {
	if (ImGui::BeginPopupModal("##modalObjectiveSelector", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar)) {

		if (ImGui::BeginMenu(Localization::getInstance().get("objective_categories.world").c_str())) {

			if (ImGui::MenuItem(Localization::getInstance().get("objective.checkpoint").c_str())) {
				const auto & position = FindPlayerPed()->GetPosition();
				float pos[3] = {position.x, position.y, CWorld::FindGroundZFor3DCoord(position.x, position.y, position.z, nullptr, nullptr)};
				ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewObjectives<CheckpointObjective>(pos);
				this->currentElement = this->getListSize() - 1;
			}

			ImGui::EndMenu();
		}


		ImGui::EndPopup();
	}
}

void Windows::ObjectivesWindow::swapItems(const int firstItem, const int secondItem) {
	const auto it = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getObjectives().begin();
	std::iter_swap(it + firstItem, it + secondItem);
}

void Windows::ObjectivesWindow::close() {
	ListWindow::close();
	TheCamera.Restore();
}

Windows::ObjectivesWindow::ObjectivesWindow(): ListWindow() {
	this->dragsItems_ = true;
}

void Windows::ObjectivesWindow::selectElement(int i) {
	ListWindow::selectElement(i);
}
