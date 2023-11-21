#include "ObjectObjective.h"

#include <CMessages.h>
#include <CRadar.h>
#include <extensions/ScriptCommands.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_stdlib.h"
#include "ProjectsService.h"
#include "strUtils.h"
#include "utils.h"
#include "../Windows/utilsRender.h"

using namespace plugin;

int ObjectObjective::spawnBlip(CObject *object) {
	int handle;
	Command<Commands::ADD_BLIP_FOR_OBJECT>(object, &handle);
	CRadar::ChangeBlipColour(handle, this->colorBlip_ - 1);
	return handle;
}

void ObjectObjective::draw(Localization &local, std::vector<std::string> &listOverlay) {
	const auto &objects = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getObjects();
	const int indexObject = utils::indexByUuid(objects, this->objectUuid_);

	IncorrectHighlight(indexObject == -1, [&] {
		utils::Combo(local.get("entities.object").c_str(), &this->objectUuid_, indexObject, objects.size(),
		             [&objects](const int i) {
			             return std::ref(objects.at(i)->getName());
		             }, [&objects](const int i) {
			             return objects.at(i)->getUuid();
		             });
	});

	ImGui::Separator();

	ImGui::InputText(local.get("general.text").c_str(), &this->text);
	ImGui::DragFloat(local.get("general.time").c_str(), &this->textTime_, 0.001f);

	ImGui::Separator();

	if (utils::Combo(local.get("general.color_marker").c_str(), &this->colorBlip_,
	                 local.getArray("general.color_marker_enum"), 6))
		this->spawnEditorBlip();

	if (indexObject != -1) {
		listOverlay.emplace_back(local.get("info_overlay.camera_view"));
		listOverlay.emplace_back(local.get("info_overlay.depend_zoom"));

		auto position = objects.at(indexObject)->getPosition();
		utils::controlCamera({position[0], position[1], position[2]});
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

ktwait ObjectObjective::execute(Scene *scene, Result &result, ktcoro_tasklist &tasklist) {
	const auto &objects = scene->getObjects();
	const int indexObject = utils::indexByUuid(objects, this->objectUuid_);

	if (indexObject == -1) {
		setObjectiveError(result, *this, NotSelected, "The object for the objective is not selected.");
		co_return;
	}

	const auto &object = objects.at(indexObject);

	if (!object->getProjectObject().has_value()) {
		setObjectiveError(result, *this, NotExists, "The entity of the object does not exist.");
		co_return;
	}

	removeProjectBlip();
	if (this->colorBlip_ > 0)
		this->projectBlip_ = spawnBlip(object->getProjectObject().value());

	auto cp1251Text = utf8ToCp1251(this->text);
	gxtEncode(cp1251Text);
	this->gameText = cp1251Text;

	CMessages::AddMessage(const_cast<char*>(this->gameText.c_str()), static_cast<unsigned>(this->textTime_ * 1000.0f),
	                      0, false);

	co_await this->execute(scene, object.get(), result, tasklist);

	removeProjectBlip();
}

void ObjectObjective::spawnEditorBlip() {
	removeEditorBlip();

	const auto &objects = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getObjects();
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

std::string& ObjectObjective::getText() {
	return text;
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
