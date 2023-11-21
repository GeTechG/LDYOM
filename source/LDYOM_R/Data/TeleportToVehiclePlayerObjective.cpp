#include "TeleportToVehiclePlayerObjective.h"

#include <extensions/ScriptCommands.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "ProjectsService.h"
#include "strUtils.h"
#include "utils.h"
#include "../Windows/utilsRender.h"

TeleportToVehiclePlayerObjective::TeleportToVehiclePlayerObjective(void *_new): BaseObjective(_new) {
	const auto suffix = fmt::format(" : {}", Localization::getInstance().get("objective.teleport_to_vehicle_player"));
	this->name += suffix;
}

boost::uuids::uuid& TeleportToVehiclePlayerObjective::getVehicleUuid() { return vehicleUuid_; }

int& TeleportToVehiclePlayerObjective::getPlace() { return place_; }

void TeleportToVehiclePlayerObjective::draw(Localization &local, std::vector<std::string> &listOverlay) {
	const auto &vehicles = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getVehicles();
	const int indexVehicle = utils::indexByUuid(vehicles, this->vehicleUuid_);

	IncorrectHighlight(indexVehicle == -1, [&] {
		utils::Combo(local.get("entities.vehicle").c_str(), &this->vehicleUuid_, indexVehicle, vehicles.size(),
		             [&vehicles](const int i) {
			             return std::ref(vehicles.at(i)->getName());
		             }, [&vehicles](const int i) {
			             return vehicles.at(i)->getUuid();
		             });
	});
	ImGui::SliderInt(local.get("general.place").c_str(), &this->place_, 0, 3,
	                 local.getArray("general.places_in_car")[this->place_].c_str());
}

ktwait TeleportToVehiclePlayerObjective::execute(Scene *scene, Result &result, ktcoro_tasklist &tasklist) {
	using namespace plugin;

	const auto &vehicles = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getVehicles();
	const int indexVehicle = utils::indexByUuid(vehicles, this->vehicleUuid_);

	if (indexVehicle == -1) {
		setObjectiveError(result, *this, NotSelected, "The vehicle for the objective is not selected.");
		co_return;
	}

	const auto &vehicle = vehicles.at(indexVehicle);

	if (!vehicle->getProjectVehicle().has_value()) {
		setObjectiveError(result, *this, NotExists, "The entity of the vehicle does not exist.");
		co_return;
	}

	if (this->place_ == 0)
		Command<Commands::TASK_WARP_CHAR_INTO_CAR_AS_DRIVER>(static_cast<CPed*>(FindPlayerPed()),
		                                                     vehicle->getProjectVehicle().value());
	else
		Command<Commands::TASK_WARP_CHAR_INTO_CAR_AS_PASSENGER>(static_cast<CPed*>(FindPlayerPed()),
		                                                        vehicle->getProjectVehicle().value(), this->place_ - 1);

	co_return;
}
