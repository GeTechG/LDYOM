#include "VehicleObjective.h"

#include <CMessages.h>
#include <CRadar.h>
#include <extensions/ScriptCommands.h>

#include "imgui.h"
#include "ProjectsService.h"
#include "strUtils.h"
#include "utils.h"
#include "../Windows/utilsRender.h"

using namespace plugin;

int VehicleObjective::spawnBlip(CVehicle* vehicle) {
	int handle;
	Command<Commands::ADD_BLIP_FOR_CAR>(vehicle, &handle);
	if (this->colorBlip_ != 10) {
		CRadar::ChangeBlipColour(handle, this->colorBlip_ - 1);
	} else {
		CRadar::SetBlipFriendly(handle, 1);
	}
	return handle;
}

void VehicleObjective::draw(Localization& local) {
	const auto& vehicles = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getVehicles();
	const int indexVehicle = utils::indexByUuid(vehicles, this->vehicleUuid_);

	IncorrectHighlight(indexVehicle == -1, [&] {
		utils::Combo(local.get("entities.vehicle").c_str(), &this->vehicleUuid_, indexVehicle, static_cast<int>(vehicles.size()), [&vehicles](const int i) {
			return vehicles.at(i)->getName();
			}, [&vehicles](const int i) {
				return vehicles.at(i)->getUuid();
			});
	});

	ImGui::Separator();

	ImGui::InputText(local.get("general.text").c_str(), this->text_.data(), sizeof this->text_);
	ImGui::DragFloat(local.get("general.time").c_str(), &this->textTime_, 0.001f);

	ImGui::Separator();

	if (utils::Combo(local.get("general.color_marker").c_str(), &this->colorBlip_, local.getArray("general.color_marker_enum")))
		this->spawnEditorBlip();

	if (indexVehicle != -1) {
		constexpr ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
		ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
		if (ImGui::Begin("##controlOverlay", nullptr, windowFlags)) {
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 16.5f);
			ImGui::Text(local.get("info_overlay.camera_view").c_str());
			ImGui::Text(local.get("info_overlay.depend_zoom").c_str());
			ImGui::PopTextWrapPos();
		}
		ImGui::End();

		auto position = vehicles.at(indexVehicle)->getPosition();
		utils::controlCamera({ position[0], position[1], position[2] });
	}
}

void VehicleObjective::open() {
	BaseObjective::open();
	this->spawnEditorBlip();
}

void VehicleObjective::close() {
	BaseObjective::close();
	this->removeEditorBlip();
}

ktwait VehicleObjective::execute(Scene* scene, Result& result, ktcoro_tasklist& tasklist) {
	const auto& vehicles = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getVehicles();
	const int indexVehicle = utils::indexByUuid(vehicles, this->vehicleUuid_);

	if (indexVehicle == -1) {
		setObjectiveError(result, *this, NotSelected, "The vehicle for the objective is not selected.");
		co_return;
	}

	const auto& vehicle = vehicles.at(indexVehicle);

	if (!vehicle->getProjectVehicle().has_value()) {
		setObjectiveError(result, *this, NotExists, "The entity of the vehicle does not exist.");
		co_return;
	}

	removeProjectBlip();
	if (this->colorBlip_ > 0) 
		this->projectBlip_ = spawnBlip(vehicle->getProjectVehicle().value());

	auto cp1251Text = utf8ToCp1251(this->text_.data());
	gxtEncode(cp1251Text);
	strlcpy(this->gameText_.data(), cp1251Text.c_str(), sizeof this->gameText_);

	CMessages::AddMessage(this->gameText_.data(), static_cast<unsigned>(this->textTime_ * 1000.0f), 0, false);

	co_await this->execute(scene, vehicle.get(), result, tasklist);

	removeProjectBlip();
}

void VehicleObjective::spawnEditorBlip() {
	removeEditorBlip();

	const auto& vehicles = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getVehicles();
	const int indexVehicle = utils::indexByUuid(vehicles, this->vehicleUuid_);

	if (indexVehicle != -1) {
		if (const auto ped = vehicles.at(indexVehicle)->getEditorVehicle(); ped.has_value()) 
			this->editorBlip_ = spawnBlip(ped.value());
	}
}

void VehicleObjective::removeEditorBlip() {
	if (this->editorBlip_.has_value())
		Command<Commands::REMOVE_BLIP>(this->editorBlip_.value());
	this->editorBlip_ = std::nullopt;
}

void VehicleObjective::removeProjectBlip() {
	if (this->projectBlip_.has_value())
		Command<Commands::REMOVE_BLIP>(this->projectBlip_.value());
	this->projectBlip_ = std::nullopt;
}

boost::uuids::uuid& VehicleObjective::getVehicleUuid() {
	return vehicleUuid_;
}

std::array<char, TEXT_SIZE>& VehicleObjective::getText() {
	return text_;
}

float& VehicleObjective::getTextTime() {
	return textTime_;
}

int& VehicleObjective::getColorBlip() {
	return colorBlip_;
}

std::optional<int>& VehicleObjective::getEditorBlip() {
	return editorBlip_;
}

std::optional<int>& VehicleObjective::getProjectBlip() {
	return projectBlip_;
}

VehicleObjective::~VehicleObjective() {
	this->removeEditorBlip();
	this->removeProjectBlip();
}
