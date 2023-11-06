#include "PickupObjective.h"

#include <CMessages.h>
#include <CRadar.h>
#include <extensions/ScriptCommands.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "ProjectsService.h"
#include "strUtils.h"
#include "utils.h"
#include "../Windows/utilsRender.h"

using namespace plugin;

int PickupObjective::spawnBlip(int pickup) {
	int handle;
	Command<Commands::ADD_BLIP_FOR_PICKUP>(pickup, &handle);
	CRadar::ChangeBlipColour(handle, this->colorBlip_ - 1);
	return handle;
}

void PickupObjective::draw(Localization &local, std::vector<std::string> &listOverlay) {
	const auto &pickups = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getPickups();
	const int indexPickup = utils::indexByUuid(pickups, this->pickupUuid_);

	IncorrectHighlight(indexPickup == -1, [&] {
		utils::Combo(local.get("entities.pickup").c_str(), &this->pickupUuid_, indexPickup, pickups.size(),
		             [&pickups](const int i) {
			             return pickups.at(i)->getName();
		             }, [&pickups](const int i) {
			             return pickups.at(i)->getUuid();
		             });
	});

	ImGui::Separator();

	ImGui::InputText(local.get("general.text").c_str(), this->text_.data(), sizeof this->text_);
	ImGui::DragFloat(local.get("general.time").c_str(), &this->textTime_, 0.001f);

	ImGui::Separator();

	if (utils::Combo(local.get("general.color_marker").c_str(), &this->colorBlip_,
	                 local.getArray("general.color_marker_enum"), 6))
		this->spawnEditorBlip();

	if (indexPickup != -1) {
		listOverlay.emplace_back(local.get("info_overlay.camera_view"));
		listOverlay.emplace_back(local.get("info_overlay.depend_zoom"));

		auto position = pickups.at(indexPickup)->getPosition();
		utils::controlCamera({position[0], position[1], position[2]});
	}
}

void PickupObjective::open() {
	BaseObjective::open();
	this->spawnEditorBlip();
}

void PickupObjective::close() {
	BaseObjective::close();
	this->removeEditorBlip();
}

ktwait PickupObjective::execute(Scene *scene, Result &result, ktcoro_tasklist &tasklist) {
	const auto &pickups = scene->getPickups();
	const int indexPickup = utils::indexByUuid(pickups, this->pickupUuid_);

	if (indexPickup == -1) {
		setObjectiveError(result, *this, NotSelected, "The pickup for the objective is not selected.");
		co_return;
	}

	const auto &pickup = pickups.at(indexPickup);

	if (!pickup->getProjectPickup().has_value()) {
		setObjectiveError(result, *this, NotExists, "The entity of the pickup does not exist.");
		co_return;
	}

	removeProjectBlip();
	if (this->colorBlip_ > 0)
		this->projectBlip_ = spawnBlip(pickup->getProjectPickupIndex().value());

	auto cp1251Text = utf8ToCp1251(this->text_.data());
	gxtEncode(cp1251Text);
	strlcpy(this->gameText_.data(), cp1251Text.c_str(), sizeof this->gameText_);

	CMessages::AddMessage(this->gameText_.data(), static_cast<unsigned>(this->textTime_ * 1000.0f), 0, false);

	co_await this->execute(scene, pickup.get(), result, tasklist);

	removeProjectBlip();
}

void PickupObjective::spawnEditorBlip() {
	removeEditorBlip();

	const auto &pickups = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getPickups();
	const int indexPickup = utils::indexByUuid(pickups, this->pickupUuid_);

	if (indexPickup != -1 && this->colorBlip_ > 0) {
		if (const auto pickup = pickups.at(indexPickup)->getEditorPickupIndex(); pickup.has_value())
			this->editorBlip_ = spawnBlip(pickup.value());
	}
}

void PickupObjective::removeEditorBlip() {
	if (this->editorBlip_.has_value())
		Command<Commands::REMOVE_BLIP>(this->editorBlip_.value());
	this->editorBlip_ = std::nullopt;
}

void PickupObjective::removeProjectBlip() {
	if (this->projectBlip_.has_value())
		Command<Commands::REMOVE_BLIP>(this->projectBlip_.value());
	this->projectBlip_ = std::nullopt;
}

boost::uuids::uuid& PickupObjective::getPickupUuid() {
	return pickupUuid_;
}

std::array<char, TEXT_SIZE>& PickupObjective::getText() {
	return text_;
}

float& PickupObjective::getTextTime() {
	return textTime_;
}

int& PickupObjective::getColorBlip() {
	return colorBlip_;
}

std::optional<int>& PickupObjective::getEditorBlip() {
	return editorBlip_;
}

std::optional<int>& PickupObjective::getProjectBlip() {
	return projectBlip_;
}

PickupObjective::~PickupObjective() {
	this->removeEditorBlip();
	this->removeProjectBlip();
}
