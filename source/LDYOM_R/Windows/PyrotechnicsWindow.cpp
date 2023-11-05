#include "PyrotechnicsWindow.h"

#include <CCamera.h>
#include <extensions/ScriptCommands.h>

#include "fa.h"
#include "imgui.h"
#include "utils.h"
#include "utilsRender.h"
#include "fmt/core.h"

std::string Windows::PyrotechnicsWindow::getNameList() {
	return fmt::format("{} {}##list", ICON_FA_FIRE_ALT, Localization::getInstance().get("entities.pyrotechnics"));
}

std::string Windows::PyrotechnicsWindow::getNameOption() {
	return fmt::format("{} {}", ICON_FA_FIRE_ALT, Localization::getInstance().get("entities.pyrotechnics"));
}

int Windows::PyrotechnicsWindow::getListSize() {
	return static_cast<int>(ProjectsService::getInstance()
	                        .getCurrentProject()
	                        .getCurrentScene()
	                        ->getPyrotechnics()
	                        .size());
}

void Windows::PyrotechnicsWindow::createNewElement() {
	ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewPyrotechnics();
}

void Windows::PyrotechnicsWindow::createNewElementFrom(int i) {
	const auto &pyrotechnics = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getPyrotechnics().
	                                                          at(i);
	ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewPyrotechnicsFrom(*pyrotechnics);
	ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getPyrotechnics().back()->
	                               spawnEditorPyrotechnics();
}

char* Windows::PyrotechnicsWindow::getElementName(int i) {
	return ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getPyrotechnics().at(i)->getName();
}

void Windows::PyrotechnicsWindow::deleteElement(int i) {
	ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getPyrotechnics().at(i)->
	                               deleteEditorPyrotechnics();

	const auto begin = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getPyrotechnics().begin();
	ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getPyrotechnics().erase(begin + i);
	this->currentElement--;
}

void Windows::PyrotechnicsWindow::drawOptions() {
	auto &local = Localization::getInstance();

	Pyrotechnics *pyrotechnics = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getPyrotechnics()
	                                                           .at(this->currentElement).get();

	//position
	DragPosition(pyrotechnics->getPosition(), [pyrotechnics] {
		pyrotechnics->updateLocation();
	});

	ImGui::Separator();

	if (ImGui::SliderInt(local.get("general.type").c_str(), &pyrotechnics->getType(), 0, 1,
	                     local.getArray("pyrotechnics.types")[pyrotechnics->getType()].c_str())) {
		pyrotechnics->spawnEditorPyrotechnics();
	}
	if (pyrotechnics->getType() == 0) {
		if (ImGui::SliderInt(local.get("pyrotechnics.size_fire").c_str(), &pyrotechnics->getSizeFire(), 0, 5)) {
			pyrotechnics->spawnEditorPyrotechnics();
		}
		ImGui::SetNextItemWidth(120);
		ImGui::DragInt(local.get("pyrotechnics.propagation_fire").c_str(), &pyrotechnics->getPropagationFire(), 0.1f, 0,
		               36);
	} else {
		ImGui::DragInt(local.get("pyrotechnics.type_explosion").c_str(), &pyrotechnics->getTypeExplosion(), 0.1f, 0,
		               13);
	}

	ObjectiveDependentInput(pyrotechnics);

	this->listOverlays.emplace_back(local.get("info_overlay.camera_view"));
	this->listOverlays.emplace_back(local.get("info_overlay.depend_zoom"));
	this->listOverlays.emplace_back(local.get("info_overlay.move_element"));

	if (utils::controlCameraWithMove(pyrotechnics->getPosition())) {
		pyrotechnics->updateLocation();
	}
}

void Windows::PyrotechnicsWindow::close() {
	ListWindow::close();
	TheCamera.Restore();
	plugin::Command<plugin::Commands::SET_PLAYER_CONTROL>(0, true);
	plugin::Command<plugin::Commands::SET_CHAR_PROOFS>(static_cast<CPed*>(FindPlayerPed()), 1, 1, 1, 1, 1);
}

void Windows::PyrotechnicsWindow::open() {
	ListWindow::open();
	plugin::Command<plugin::Commands::SET_PLAYER_CONTROL>(0, false);
	for (const auto &pyrotechnics : ProjectsService::getInstance().getCurrentProject().getCurrentScene()->
	                                                               getPyrotechnics()) {
		pyrotechnics->spawnEditorPyrotechnics();
	}
}
