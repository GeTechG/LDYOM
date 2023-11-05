#include "ActorObjective.h"

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

int ActorObjective::spawnBlip(CPed *ped) {
	int handle;
	Command<Commands::ADD_BLIP_FOR_CHAR>(ped, &handle);
	if (this->colorBlip_ != 10) {
		CRadar::ChangeBlipColour(handle, this->colorBlip_ - 1);
	} else {
		CRadar::SetBlipFriendly(handle, 1);
	}
	return handle;
}

void ActorObjective::draw(Localization &local, std::vector<std::string> &listOverlay) {
	const auto &actors = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getActors();
	const int indexActor = utils::indexByUuid(actors, this->actorUuid_);

	IncorrectHighlight(indexActor == -1, [&] {
		utils::Combo(local.get("entities.actor").c_str(), &this->actorUuid_, indexActor, actors.size(),
		             [&actors](const int i) {
			             return actors.at(i)->getName();
		             }, [&actors](const int i) {
			             return actors.at(i)->getUuid();
		             });
	});

	ImGui::Separator();

	ImGui::InputText(local.get("general.text").c_str(), this->text_.data(), sizeof this->text_);
	ImGui::DragFloat(local.get("general.time").c_str(), &this->textTime_, 0.001f);

	ImGui::Separator();

	if (utils::Combo(local.get("general.color_marker").c_str(), &this->colorBlip_,
	                 local.getArray("general.color_marker_enum")))
		this->spawnEditorBlip();

	if (indexActor != -1) {
		listOverlay.emplace_back(local.get("info_overlay.camera_view"));
		listOverlay.emplace_back(local.get("info_overlay.depend_zoom"));

		auto position = actors.at(indexActor)->getPosition();
		utils::controlCamera({position[0], position[1], position[2]});
	}
}

void ActorObjective::open() {
	BaseObjective::open();
	this->spawnEditorBlip();
}

void ActorObjective::close() {
	BaseObjective::close();
	this->removeEditorBlip();
}

ktwait ActorObjective::execute(Scene *scene, Result &result, ktcoro_tasklist &tasklist) {
	const auto &actors = scene->getActors();
	const int indexActor = utils::indexByUuid(actors, this->actorUuid_);

	if (indexActor == -1) {
		setObjectiveError(result, *this, NotSelected, "The actor for the objective is not selected.");
		co_return;
	}

	const auto &actor = actors.at(indexActor);

	if (!actor->getProjectPed().has_value()) {
		setObjectiveError(result, *this, NotExists, "The entity of the actor does not exist.");
		co_return;
	}

	removeProjectBlip();
	if (this->colorBlip_ > 0)
		this->projectBlip_ = spawnBlip(actor->getProjectPed().value());

	auto cp1251Text = utf8ToCp1251(this->text_.data());
	gxtEncode(cp1251Text);
	strlcpy(this->gameText_.data(), cp1251Text.c_str(), sizeof this->gameText_);

	CMessages::AddMessage(this->gameText_.data(), static_cast<unsigned>(this->textTime_ * 1000.0f), 0, false);

	co_await this->execute(scene, actor.get(), result, tasklist);

	removeProjectBlip();
}

void ActorObjective::spawnEditorBlip() {
	removeEditorBlip();

	const auto &actors = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getActors();
	const int indexActor = utils::indexByUuid(actors, this->actorUuid_);

	if (indexActor != -1 && this->colorBlip_ > 0) {
		if (const auto ped = actors.at(indexActor)->getEditorPed(); ped.has_value())
			this->editorBlip_ = spawnBlip(ped.value());
	}
}

void ActorObjective::removeEditorBlip() {
	if (this->editorBlip_.has_value())
		Command<Commands::REMOVE_BLIP>(this->editorBlip_.value());
	this->editorBlip_ = std::nullopt;
}

void ActorObjective::removeProjectBlip() {
	if (this->projectBlip_.has_value())
		Command<Commands::REMOVE_BLIP>(this->projectBlip_.value());
	this->projectBlip_ = std::nullopt;
}

boost::uuids::uuid& ActorObjective::getActorUuid() {
	return actorUuid_;
}

std::array<char, TEXT_SIZE>& ActorObjective::getText() {
	return text_;
}

float& ActorObjective::getTextTime() {
	return textTime_;
}

int& ActorObjective::getColorBlip() {
	return colorBlip_;
}

std::optional<int>& ActorObjective::getEditorBlip() {
	return editorBlip_;
}

std::optional<int>& ActorObjective::getProjectBlip() {
	return projectBlip_;
}

ActorObjective::~ActorObjective() {
	this->removeEditorBlip();
	this->removeProjectBlip();
}
