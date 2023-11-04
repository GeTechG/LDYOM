#include "EnterVehicleActorObjective.h"

#include <extensions/ScriptCommands.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "ProjectsService.h"
#include "strUtils.h"
#include "utils.h"
#include "../Windows/utilsRender.h"

EnterVehicleActorObjective::EnterVehicleActorObjective(void *_new): BaseObjective(_new) {
	const auto suffix = fmt::format(" : {}", Localization::getInstance().get("objective.enter_vehicle_actor"));
	strlcat(this->name_.data(), suffix.c_str(), sizeof this->name_);
}

void EnterVehicleActorObjective::draw(Localization &local) {
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

	const auto &vehicles = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getVehicles();
	const int indexVehicle = utils::indexByUuid(vehicles, this->vehicleUuid_);

	IncorrectHighlight(indexVehicle == -1, [&] {
		utils::Combo(local.get("entities.vehicle").c_str(), &this->vehicleUuid_, indexVehicle, vehicles.size(),
		             [&vehicles](const int i) {
			             return vehicles.at(i)->getName();
		             }, [&vehicles](const int i) {
			             return vehicles.at(i)->getUuid();
		             });
	});

	ImGui::SliderInt(local.get("general.place").c_str(), &this->place_, 0, 3,
	                 local.getArray("general.places_in_car").at(this->place_).c_str());
	utils::ToggleButton(local.get("general.teleport").c_str(), &this->teleport_);
	utils::ToggleButton(local.get("general.wait_execute").c_str(), &this->waitExecute_);
}

ktwait EnterVehicleActorObjective::execute(Scene *scene, Actor *actor, Result &result, ktcoro_tasklist &tasklist) {
	using namespace plugin;

	const auto &vehicles = scene->getVehicles();
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

	if (this->teleport_) {
		if (this->place_ == 0) {
			Command<Commands::TASK_WARP_CHAR_INTO_CAR_AS_DRIVER>(actor->getProjectPed().value(),
			                                                     vehicle->getProjectVehicle().value());
		} else {
			Command<Commands::TASK_WARP_CHAR_INTO_CAR_AS_PASSENGER>(actor->getProjectPed().value(),
			                                                        vehicle->getProjectVehicle().value(),
			                                                        this->place_ - 1);
		}
	} else {
		if (this->place_ == 0) {
			Command<Commands::TASK_ENTER_CAR_AS_DRIVER>(actor->getProjectPed().value(),
			                                            vehicle->getProjectVehicle().value(), -1);
		} else {
			Command<Commands::TASK_ENTER_CAR_AS_PASSENGER>(actor->getProjectPed().value(),
			                                               vehicle->getProjectVehicle().value(), -1, this->place_ - 1);
		}
	}

	if (this->waitExecute_) {
		while (!Command<
			Commands::IS_CHAR_IN_CAR>(actor->getProjectPed().value(), vehicle->getProjectVehicle().value())) {
			co_await 1;
		}
	}
}
