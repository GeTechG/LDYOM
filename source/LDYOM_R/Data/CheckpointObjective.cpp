#include "CheckpointObjective.h"
#include <ktcoro_wait.hpp>

#include "imgui.h"
#include "ProjectsService.h"
#include "fmt/core.h"
#include "Localization/Localization.h"
#include "../shared/extensions/ScriptCommands.h"
#include "CRadar.h"
#include "Result.h"
#include "strUtils.h"
#include "utils.h"
#include "../Windows/utilsRender.h"

using namespace plugin;

extern bool restart;

CheckpointObjective::CheckpointObjective(void* _new): BaseObjective(nullptr),
													  WorldObjective(nullptr),
													  checkpointUuid_(boost::uuids::uuid{}), text_(""), onWhatArrive_(0), textTime_(2),
                                                      comeBackVehicle_(boost::uuids::uuid{}),
                                                      textComeBackVehicle_(""),
                                                      colorBlipComeBackVehicle_(0) {
	const auto suffix = fmt::format(" : {}", Localization::getInstance().get("objective.checkpoint"));
	strlcat(this->name_.data(), suffix.c_str(), sizeof this->name_);
}

CheckpointObjective::CheckpointObjective(const CheckpointObjective& other): BaseObjective{ other }, WorldObjective{other},
                                                                            checkpointUuid_{other.checkpointUuid_},
                                                                            text_{other.text_},
                                                                            onWhatArrive_{other.onWhatArrive_},
                                                                            textTime_{other.textTime_},
                                                                            comeBackVehicle_{other.comeBackVehicle_},
                                                                            textComeBackVehicle_{other.textComeBackVehicle_},
                                                                            colorBlipComeBackVehicle_{other.colorBlipComeBackVehicle_},
                                                                            projectComeBackBlip_{other.projectComeBackBlip_},
                                                                            gameText_{other.gameText_},
                                                                            popupSpriteBlipSelector_{other.popupSpriteBlipSelector_} {
}

CheckpointObjective& CheckpointObjective::operator=(const CheckpointObjective& other) {
	if (this == &other)
		return *this;
	WorldObjective::operator =(other);
	checkpointUuid_ = other.checkpointUuid_;
	text_ = other.text_;
	onWhatArrive_ = other.onWhatArrive_;
	textTime_ = other.textTime_;
	comeBackVehicle_ = other.comeBackVehicle_;
	textComeBackVehicle_ = other.textComeBackVehicle_;
	colorBlipComeBackVehicle_ = other.colorBlipComeBackVehicle_;
	projectComeBackBlip_ = other.projectComeBackBlip_;
	gameText_ = other.gameText_;
	popupSpriteBlipSelector_ = other.popupSpriteBlipSelector_;
	return *this;
}

CheckpointObjective::~CheckpointObjective() {
	if (!restart) {
		removeProjectComeBackBlip();
	}
}

ktwait CheckpointObjective::execute(Scene* scene, Result& result, ktcoro_tasklist& tasklist) {
	const auto& checkpoints = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getCheckpoints();
	const int indexCheckpoint = utils::indexByUuid(checkpoints, this->checkpointUuid_);

	if (indexCheckpoint == -1) {
		setObjectiveError(result, *this, NotSelected, "The checkpoint for the objective is not selected.");
		co_return;
	}

	const auto & checkpoint = checkpoints.at(indexCheckpoint);

	auto playerPed = static_cast<CPed*>(FindPlayerPed());

	auto cp1251Text = utf8ToCp1251(this->text_.data());
	gxtEncode(cp1251Text);
	strlcpy(this->gameText_.data(), cp1251Text.c_str(), sizeof this->gameText_);

	CMessages::AddMessage(this->gameText_.data(), static_cast<unsigned>(this->textTime_ * 1000.0f), 0, false);
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
				isLocate = Command<Commands::LOCATE_CHAR_ANY_MEANS_2D>(playerPed, position.x, position.y, radius, radius, false);
			else
				isLocate = Command<Commands::LOCATE_CHAR_ANY_MEANS_3D>(playerPed, position.x, position.y, position.z, radius, radius, radius, false);
			break;
		case 1:
			if (is2d)
				isLocate = Command<Commands::LOCATE_CHAR_ON_FOOT_2D>(playerPed, position.x, position.y, radius, radius, false);
			else
				isLocate = Command<Commands::LOCATE_CHAR_ON_FOOT_3D>(playerPed, position.x, position.y, position.z, radius, radius, radius, false);
			break;
		case 2:
			if (is2d)
				isLocate = Command<Commands::LOCATE_CHAR_IN_CAR_2D>(playerPed, position.x, position.y, radius, radius, false);
			else
				isLocate = Command<Commands::LOCATE_CHAR_IN_CAR_3D>(playerPed, position.x, position.y, position.z, radius, radius, radius, false);
			break;
		case 3: {
			auto vehicle = scene->getVehicles().at(vehicleIdx)->getProjectVehicle().value();
			bool isPlayerInCar = Command<Commands::IS_CHAR_IN_CAR>(playerPed, vehicle);

			if (!isPlayerInCar) {
				checkpoint->deleteProjectBlip();

				auto cp1251TextComeBack = utf8ToCp1251(this->textComeBackVehicle_.data());
				gxtEncode(cp1251TextComeBack);
				CMessages::AddMessageJumpQ(const_cast<char*>(cp1251TextComeBack.c_str()), 2000u, 0, true);

				int blipComeBack;

				if (this->colorBlipComeBackVehicle_ > 0) {
					Command<Commands::ADD_BLIP_FOR_CAR>(vehicle, &blipComeBack);
					this->projectComeBackBlip_ = blipComeBack;
					if (this->colorBlipComeBackVehicle_ != 10) {
						CRadar::ChangeBlipColour(blipComeBack, this->colorBlipComeBackVehicle_ - 1);
					} else {
						CRadar::SetBlipFriendly(blipComeBack, 1);
					}
				}

				while(!isPlayerInCar) {
					isPlayerInCar = Command<Commands::IS_CHAR_IN_CAR>(playerPed, vehicle);
					co_await 1;
				}
				this->removeProjectComeBackBlip();

				CMessages::AddMessageJumpQ(this->gameText_.data(), static_cast<unsigned>(this->textTime_ * 1000.0f), 0, false);
				checkpoint->spawnProjectBlip();
			}

			if (is2d)
				isLocate = Command<Commands::LOCATE_CHAR_IN_CAR_2D>(playerPed, position.x, position.y, radius, radius, false);
			else
				isLocate = Command<Commands::LOCATE_CHAR_IN_CAR_3D>(playerPed, position.x, position.y, position.z, radius, radius, radius, false);
			break;
		}
		default: 
			break;
		}

		co_await 1;
	}

	this->removeProjectComeBackBlip();
}

void CheckpointObjective::draw(Localization& local) {

	const auto& checkpoints = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getCheckpoints();
	const int indexCheckpoint = utils::indexByUuid(checkpoints, this->checkpointUuid_);

	IncorrectHighlight(indexCheckpoint == -1, [&] {
		utils::Combo(local.get("entities.checkpoint").c_str(), &this->checkpointUuid_, indexCheckpoint, static_cast<int>(checkpoints.size()), [&checkpoints](const int i) {
			return checkpoints.at(i)->getName();
			}, [&checkpoints](const int i) {
				return checkpoints.at(i)->getUuid();
			});
	});

	ImGui::InputText(local.get("general.text").c_str(), this->text_.data(), sizeof this->text_);
	ImGui::DragFloat(local.get("general.time").c_str(), &this->textTime_, 0.001f);
	ImGui::Separator();
	utils::Combo(local.get("checkpoint_objective.how_to_arrive").c_str(), &this->onWhatArrive_, local.getArray("checkpoint_objective.how_to_arrive_enum"));

	if (this->onWhatArrive_ == 3) {
		ImGui::PushID("##oArriveOnVeh");
		const auto & vehicles = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getVehicles();

		const int indexVehicle = utils::indexByUuid(vehicles, this->comeBackVehicle_);
		IncorrectHighlight(indexVehicle == -1, [&] {
			utils::Combo(local.get("entities.vehicle").c_str(), &this->comeBackVehicle_, indexVehicle, static_cast<int>(vehicles.size()), [&vehicles](const int i) {
				return vehicles.at(i)->getName();
				}, [&vehicles](const int i) {
					return vehicles.at(i)->getUuid();
				});
		});
		ImGui::InputText(local.get("general.text").c_str(), this->textComeBackVehicle_.data(), sizeof this->textComeBackVehicle_);
		utils::Combo(local.get("general.color_marker").c_str(), &this->colorBlipComeBackVehicle_, local.getArray("general.color_marker_enum"));
		ImGui::PopID();
	}

	if (indexCheckpoint != -1) {
		constexpr ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
		ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
		if (ImGui::Begin("##controlOverlay", nullptr, windowFlags)) {
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 16.5f);
			ImGui::Text(local.get("info_overlay.camera_view").c_str());
			ImGui::Text(local.get("info_overlay.depend_zoom").c_str());
			ImGui::PopTextWrapPos();
		}
		ImGui::End();

		auto position = checkpoints.at(indexCheckpoint)->getPosition();
		utils::controlCamera({position[0], position[1], position[2]});
	}
}

boost::uuids::uuid& CheckpointObjective::getCheckpointUuid() {
	return checkpointUuid_;
}

std::array<char, TEXT_SIZE>& CheckpointObjective::getText() {
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

std::array<char, TEXT_SIZE>& CheckpointObjective::getTextComeBackVehicle() {
	return textComeBackVehicle_;
}

int& CheckpointObjective::getColorBlipComeBackVehicle() {
	return colorBlipComeBackVehicle_;
}

std::optional<int>& CheckpointObjective::getProjectComeBackBlip() {
	return projectComeBackBlip_;
}

std::array<char, TEXT_SIZE>& CheckpointObjective::getGameText() {
	return gameText_;
}

void CheckpointObjective::removeProjectComeBackBlip() {
	if (this->projectComeBackBlip_.has_value())
		Command<Commands::REMOVE_BLIP>(this->projectComeBackBlip_.value());
	this->projectComeBackBlip_ = std::nullopt;
}


