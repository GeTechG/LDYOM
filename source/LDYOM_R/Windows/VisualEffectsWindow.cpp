#include "VisualEffectsWindow.h"

#include <CCamera.h>
#include <extensions/ScriptCommands.h>

#include "fa.h"
#include "imgui.h"
#include "utils.h"
#include "utilsRender.h"
#include "fmt/core.h"

std::string Windows::VisualEffectsWindow::getNameList() {
	return fmt::format("{} {}", ICON_FA_SPARKLES, Localization::getInstance().get("entities.visual_effects"));
}

std::string Windows::VisualEffectsWindow::getNameOption() {
	return fmt::format("{} {}", ICON_FA_SPARKLES, Localization::getInstance().get("entities.visual_effect"));
}

int Windows::VisualEffectsWindow::getListSize() {
	return static_cast<int>(ProjectsService::getInstance()
	                        .getCurrentProject()
	                        .getCurrentScene()
	                        ->getVisualEffects()
	                        .size());
}

void Windows::VisualEffectsWindow::createNewElement() {
	ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewVisualEffect();
}

void Windows::VisualEffectsWindow::createNewElementFrom(int i) {
	const auto &checkpoint = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getVisualEffects().
	                                                        at(i);
	ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewVisualEffectFrom(*checkpoint);
	ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getVisualEffects().back()->
	                               spawnEditorVisualEffect();
}

std::string& Windows::VisualEffectsWindow::getElementName(int i) {
	return ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getVisualEffects().at(i)->getName();
}

void Windows::VisualEffectsWindow::deleteElement(int i) {
	ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getVisualEffects().at(i)->
	                               deleteEditorVisualEffect();

	const auto begin = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getVisualEffects().begin();
	ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getVisualEffects().erase(begin + i);
	this->currentElement--;
}

void Windows::VisualEffectsWindow::drawOptions() {
	auto &local = Localization::getInstance();

	VisualEffect *visualEffect = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->
	                                                            getVisualEffects().at(this->currentElement).get();

	//position
	DragPosition(visualEffect->getPosition(), [] {});

	ImGui::Separator();

	if (ImGui::SliderInt(local.get("general.type").c_str(), &visualEffect->getType(), 0, 1,
	                     local.getArray("visual_effect.types")[visualEffect->getType()].c_str())) {
		visualEffect->getEffectType() = 0;
	}

	ImGui::DragFloat(local.get("general.size").c_str(), &visualEffect->getSize(), 0.01f);

	if (visualEffect->getType() == 0) {
		ImGui::SliderInt(local.get("visual_effect.effect_type").c_str(), &visualEffect->getEffectType(), 0, 9);
		ImGui::SliderInt(local.get("visual_effect.flare").c_str(), &visualEffect->getFlare(), 0, 2);
	} else {
		ImGui::SliderInt(local.get("visual_effect.effect_type").c_str(), &visualEffect->getEffectType(), 0, 6);
		ImGui::DragFloat(local.get("general.rotate").c_str(), &visualEffect->getAngle(), 0.1f, -180.f, 180.f, "%.2f°");
	}

	ImGui::ColorEdit4(local.get("general.color").c_str(), visualEffect->getColor().data(),
	                  ImGuiColorEditFlags_AlphaBar + ImGuiColorEditFlags_NoInputs);

	ObjectiveDependentInput(visualEffect);

	this->listOverlays.emplace_back(local.get("info_overlay.camera_view"));
	this->listOverlays.emplace_back(local.get("info_overlay.depend_zoom"));
	this->listOverlays.emplace_back(local.get("info_overlay.move_element"));

	utils::controlCameraWithMove(visualEffect->getPosition());
}

void Windows::VisualEffectsWindow::close() {
	ListWindow::close();
	TheCamera.Restore();
	plugin::Command<plugin::Commands::SET_PLAYER_CONTROL>(0, true);
	plugin::Command<plugin::Commands::SET_CHAR_PROOFS>(static_cast<CPed*>(FindPlayerPed()), 1, 1, 1, 1, 1);
}

void Windows::VisualEffectsWindow::open() {
	ListWindow::open();
	plugin::Command<plugin::Commands::SET_PLAYER_CONTROL>(0, false);
}
