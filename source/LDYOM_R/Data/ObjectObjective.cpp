﻿#include "ObjectObjective.h"

#include <CMessages.h>
#include <CRadar.h>
#include <extensions/ScriptCommands.h>

#include "imgui.h"
#include "ProjectsService.h"
#include "strUtils.h"
#include "utils.h"
#include "../Windows/utilsRender.h"

using namespace plugin;

int ObjectObjective::spawnBlip(CObject* object) {
	int handle;
	Command<Commands::ADD_BLIP_FOR_OBJECT>(object, &handle);
	if (this->colorBlip_ != 10) {
		CRadar::ChangeBlipColour(handle, this->colorBlip_ - 1);
	}
	else {
		CRadar::SetBlipFriendly(handle, 1);
	}
	return handle;
}

void ObjectObjective::draw(Localization& local) {
	const auto& objects = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getObjects();
	const int indexObject = utils::indexByUuid(objects, this->objectUuid_);

	IncorrectHighlight(indexObject == -1, [&] {
		utils::Combo(local.get("entities.object").c_str(), &this->objectUuid_, indexObject, static_cast<int>(objects.size()), [&objects](const int i) {
			return objects.at(i)->getName();
			}, [&objects](const int i) {
				return objects.at(i)->getUuid();
			});
		});

	ImGui::Separator();

	ImGui::InputText(local.get("general.text").c_str(), this->text_.data(), sizeof this->text_);
	ImGui::DragFloat(local.get("general.time").c_str(), &this->textTime_, 0.001f);

	ImGui::Separator();

	if (utils::Combo(local.get("general.color_marker").c_str(), &this->colorBlip_, local.getArray("general.color_marker_enum")))
		this->spawnEditorBlip();

	if (indexObject != -1) {
		constexpr ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
		ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
		if (ImGui::Begin("##controlOverlay", nullptr, windowFlags)) {
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 16.5f);
			ImGui::Text(local.get("info_overlay.camera_view").c_str());
			ImGui::Text(local.get("info_overlay.depend_zoom").c_str());
			ImGui::PopTextWrapPos();
		}
		ImGui::End();

		auto position = objects.at(indexObject)->getPosition();
		utils::controlCamera({ position[0], position[1], position[2] });
	}
}

void ObjectObjective::open() {
	BaseObjective::open();
	this->spawnEditorBlip();
}

void ObjectObjective::close() {
	BaseObjective::close();
	this->removeEditorBlip();
}

ktwait ObjectObjective::execute(Scene* scene, Result& result, ktcoro_tasklist& tasklist) {
	const auto& objects = scene->getObjects();
	const int indexObject = utils::indexByUuid(objects, this->objectUuid_);

	if (indexObject == -1) {
		setObjectiveError(result, *this, NotSelected, "The object for the objective is not selected.");
		co_return;
	}

	const auto& object = objects.at(indexObject);

	if (!object->getProjectObject().has_value()) {
		setObjectiveError(result, *this, NotExists, "The entity of the object does not exist.");
		co_return;
	}

	removeProjectBlip();
	if (this->colorBlip_ > 0)
		this->projectBlip_ = spawnBlip(object->getProjectObject().value());

	auto cp1251Text = utf8ToCp1251(this->text_.data());
	gxtEncode(cp1251Text);
	strlcpy(this->gameText_.data(), cp1251Text.c_str(), sizeof this->gameText_);

	CMessages::AddMessage(this->gameText_.data(), static_cast<unsigned>(this->textTime_ * 1000.0f), 0, false);

	co_await this->execute(scene, object.get(), result, tasklist);

	removeProjectBlip();
}

void ObjectObjective::spawnEditorBlip() {
	removeEditorBlip();

	const auto& objects = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getObjects();
	const int indexObject = utils::indexByUuid(objects, this->objectUuid_);

	if (indexObject != -1 && this->colorBlip_ > 0) {
		if (const auto ped = objects.at(indexObject)->getEditorObject(); ped.has_value())
			this->editorBlip_ = spawnBlip(ped.value());
	}
}

void ObjectObjective::removeEditorBlip() {
	if (this->editorBlip_.has_value())
		Command<Commands::REMOVE_BLIP>(this->editorBlip_.value());
	this->editorBlip_ = std::nullopt;
}

void ObjectObjective::removeProjectBlip() {
	if (this->projectBlip_.has_value())
		Command<Commands::REMOVE_BLIP>(this->projectBlip_.value());
	this->projectBlip_ = std::nullopt;
}

boost::uuids::uuid& ObjectObjective::getObjectUuid() {
	return objectUuid_;
}

std::array<char, TEXT_SIZE>& ObjectObjective::getText() {
	return text_;
}

float& ObjectObjective::getTextTime() {
	return textTime_;
}

int& ObjectObjective::getColorBlip() {
	return colorBlip_;
}

std::optional<int>& ObjectObjective::getEditorBlip() {
	return editorBlip_;
}

std::optional<int>& ObjectObjective::getProjectBlip() {
	return projectBlip_;
}

ObjectObjective::~ObjectObjective() {
	this->removeEditorBlip();
	this->removeProjectBlip();
}