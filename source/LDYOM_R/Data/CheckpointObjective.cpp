#include "CheckpointObjective.h"
#include <ktcoro_wait.hpp>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "CRadar.h"
#include "imgui.h"
#include "imgui_stdlib.h"
#include "ProjectsService.h"
#include "Result.h"
#include "Settings.h"
#include "strUtils.h"
#include "utils.h"
#include "../shared/extensions/ScriptCommands.h"
#include "../Windows/utilsRender.h"
#include "fmt/core.h"
#include "Localization/Localization.h"

using namespace plugin;

extern bool restart;

CheckpointObjective::CheckpointObjective(void *_new): WorldObjective(nullptr),
                                                      BaseObjective(nullptr),
                                                      checkpointUuid_(boost::uuids::uuid{}),
                                                      onWhatArrive_(0), textTime_(2),
                                                      comeBackVehicle_(boost::uuids::uuid{}),
                                                      colorBlipComeBackVehicle_(0) {
	const auto suffix = fmt::format(" : {}", Localization::getInstance().get("objective.checkpoint"));
	this->name += suffix;
}

CheckpointObjective::~CheckpointObjective() {
	if (!restart) {
		removeProjectComeBackBlip();
	}
}

ktwait CheckpointObjective::execute(Scene *scene, Result &result, ktcoro_tasklist &tasklist) {
	const auto &checkpoints = scene->getCheckpoints();
	const int indexCheckpoint = utils::indexByUuid(checkpoints, this->checkpointUuid_);

	if (indexCheckpoint == -1) {
		setObjectiveError(result, *this, NotSelected, "The checkpoint for the objective is not selected.");
		co_return;
	}

	const auto &checkpoint = checkpoints.at(indexCheckpoint);

	auto playerPed = static_cast<CPed*>(FindPlayerPed());

	auto cp1251Text = utf8ToCp1251(this->text_);
	gxtEncode(cp1251Text);
	this->gameText_ = cp1251Text;

	CMessages::AddMessage(const_cast<char*>(this->gameText_.c_str()), static_cast<unsigned>(this->textTime_ * 1000.0f),
	                      0, false);
	checkpoint->spawnProjectBlip();

	int vehicleIdx = -1;
	if (this->onWhatArrive_ == 3) {
		vehicleIdx = utils::indexByUuid(scene->getVehicles(), this->comeBackVehicle_);
		if (vehicleIdx == -1) {
			setObjectiveError(result, *this, NotSelected, "The vehicle for the objective has not been selected.");
			co_return;
		}
	}

	bool isLocate = false;
	while (!isLocate) {
		if (!checkpoint->existProjectEntity()) {
			setObjectiveError(result, *this, NotExists, "The entity of the checkpoint does not exist.");
			co_return;
		}

		auto position = checkpoint->getProjectEntityPosition();
		float radius = checkpoint->getRadius();
		bool is2d = false;
		if (checkpoint->getType() == 1 && checkpoint->getCheckpointType() <= 2) {
			is2d = true;
		}

		switch (this->onWhatArrive_) {
			case 0:
				if (is2d)
					isLocate = Command<Commands::LOCATE_CHAR_ANY_MEANS_2D>(playerPed, position.x, position.y, radius,
					                                                       radius, false);
				else
					isLocate = Command<Commands::LOCATE_CHAR_ANY_MEANS_3D>(
						playerPed, position.x, position.y, position.z,
						radius, radius, radius, false);
				break;
			case 1:
				if (is2d)
					isLocate = Command<Commands::LOCATE_CHAR_ON_FOOT_2D>(
						playerPed, position.x, position.y, radius, radius,
						false);
				else
					isLocate = Command<Commands::LOCATE_CHAR_ON_FOOT_3D>(playerPed, position.x, position.y, position.z,
					                                                     radius, radius, radius, false);
				break;
			case 2:
				if (is2d)
					isLocate = Command<Commands::LOCATE_CHAR_IN_CAR_2D>(playerPed, position.x, position.y, radius,
					                                                    radius,
					                                                    false);
				else
					isLocate = Command<Commands::LOCATE_CHAR_IN_CAR_3D>(playerPed, position.x, position.y, position.z,
					                                                    radius, radius, radius, false);
				break;
			case 3: {
				auto &vehicle = scene->getVehicles().at(vehicleIdx);
				if (!vehicle->getProjectVehicle().has_value()) {
					setObjectiveError(result, *this, NotExists, "The entity of the vehicle does not exist.");
					co_return;
				}

				const auto vehicleRef = vehicle->getProjectVehicle().value();
				bool isPlayerInCar = Command<Commands::IS_CHAR_IN_CAR>(playerPed, vehicleRef);

				if (!isPlayerInCar) {
					checkpoint->deleteProjectBlip();

					auto cp1251TextComeBack = utf8ToCp1251(this->textComeBackVehicle_);
					gxtEncode(cp1251TextComeBack);
					CMessages::AddMessageJumpQ(const_cast<char*>(cp1251TextComeBack.c_str()), 2000u, 0, true);

					int blipComeBack;

					if (this->colorBlipComeBackVehicle_ > 0) {
						Command<Commands::ADD_BLIP_FOR_CAR>(vehicleRef, &blipComeBack);
						this->projectComeBackBlip_ = blipComeBack;
						CRadar::ChangeBlipColour(blipComeBack, this->colorBlipComeBackVehicle_ - 1);
					}

					while (!isPlayerInCar) {
						isPlayerInCar = Command<Commands::IS_CHAR_IN_CAR>(playerPed, vehicleRef);
						co_await 1;
					}
					this->removeProjectComeBackBlip();

					CMessages::AddMessageJumpQ(const_cast<char*>(this->gameText_.c_str()),
					                           static_cast<unsigned>(this->textTime_ * 1000.0f),
					                           0,
					                           false);
					checkpoint->spawnProjectBlip();
				}

				if (is2d)
					isLocate = Command<Commands::LOCATE_CHAR_IN_CAR_2D>(playerPed, position.x, position.y, radius,
					                                                    radius,
					                                                    false);
				else
					isLocate = Command<Commands::LOCATE_CHAR_IN_CAR_3D>(playerPed, position.x, position.y, position.z,
					                                                    radius, radius, radius, false);
				break;
			}
			default:
				break;
		}

		co_await 1;
	}

	this->removeProjectComeBackBlip();
}

void CheckpointObjective::draw(Localization &local, std::vector<std::string> &listOverlay) {
	const auto &checkpoints = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getCheckpoints();
	const int indexCheckpoint = utils::indexByUuid(checkpoints, this->checkpointUuid_);

	IncorrectHighlight(indexCheckpoint == -1, [&] {
		utils::Combo(local.get("entities.checkpoint").c_str(), &this->checkpointUuid_, indexCheckpoint,
		             checkpoints.size(), [&checkpoints](const int i) {
			             return std::ref(checkpoints.at(i)->getName());
		             }, [&checkpoints](const int i) {
			             return checkpoints.at(i)->getUuid();
		             });
	});

	if (Settings::getInstance().get<bool>("main.autoBindRequireFields").value_or(true)) {
		if (indexCheckpoint == -1) {
			if (!checkpoints.empty()) {
				this->checkpointUuid_ = checkpoints.back()->getUuid();
			}
		}
	}

	ImGui::InputText(local.get("general.text").c_str(), &this->text_);
	ImGui::DragFloat(local.get("general.time").c_str(), &this->textTime_, 0.001f);
	ImGui::Separator();
	utils::Combo(local.get("checkpoint_objective.how_to_arrive").c_str(), &this->onWhatArrive_,
	             local.getArray("checkpoint_objective.how_to_arrive_enum"));

	if (this->onWhatArrive_ == 3) {
		ImGui::PushID("##oArriveOnVeh");
		const auto &vehicles = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getVehicles();
		const int indexVehicle = utils::indexByUuid(vehicles, this->comeBackVehicle_);

		IncorrectHighlight(indexVehicle == -1, [&] {
			utils::Combo(local.get("entities.vehicle").c_str(), &this->comeBackVehicle_, indexVehicle, vehicles.size(),
			             [&vehicles](const int i) {
				             return std::ref(vehicles.at(i)->getName());
			             }, [&vehicles](const int i) {
				             return vehicles.at(i)->getUuid();
			             });
		});

		if (Settings::getInstance().get<bool>("main.autoBindRequireFields").value_or(true)) {
			if (indexVehicle == -1) {
				if (!vehicles.empty()) {
					this->comeBackVehicle_ = vehicles.back()->getUuid();
				}
			}
		}

		ImGui::InputText(local.get("checkpoint_objective.comeBackText").c_str(), &this->textComeBackVehicle_);
		utils::Combo(local.get("general.color_marker").c_str(), &this->colorBlipComeBackVehicle_,
		             local.getArray("general.color_marker_enum"), 6);
		ImGui::PopID();
	}

	if (indexCheckpoint != -1) {
		listOverlay.emplace_back(local.get("info_overlay.camera_view"));
		listOverlay.emplace_back(local.get("info_overlay.depend_zoom"));


		auto position = checkpoints.at(indexCheckpoint)->getPosition();
		utils::controlCamera({position[0], position[1], position[2]});
	}
}

boost::uuids::uuid& CheckpointObjective::getCheckpointUuid() {
	return checkpointUuid_;
}

std::string& CheckpointObjective::getText() {
	return text_;
}

int& CheckpointObjective::getOnWhatArrive() {
	return onWhatArrive_;
}

float& CheckpointObjective::getTextTime() {
	return textTime_;
}

boost::uuids::uuid& CheckpointObjective::getComeBackVehicle() {
	return comeBackVehicle_;
}

std::string& CheckpointObjective::getTextComeBackVehicle() {
	return textComeBackVehicle_;
}

int& CheckpointObjective::getColorBlipComeBackVehicle() {
	return colorBlipComeBackVehicle_;
}

std::optional<int>& CheckpointObjective::getProjectComeBackBlip() {
	return projectComeBackBlip_;
}

std::string& CheckpointObjective::getGameText() {
	return gameText_;
}

void CheckpointObjective::removeProjectComeBackBlip() {
	if (this->projectComeBackBlip_.has_value())
		Command<Commands::REMOVE_BLIP>(this->projectComeBackBlip_.value());
	this->projectComeBackBlip_ = std::nullopt;
}
