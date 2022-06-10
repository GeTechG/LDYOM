#include "CheckpointObjective.h"

#include <C3dMarkers.h>
#include <cstring>
#include <ktcoro_wait.hpp>

#include "imgui.h"
#include "ProjectsService.h"
#include "fmt/core.h"
#include "Localization/Localization.h"
#include "../Windows/utilsRender.h"
#include "../shared/extensions/ScriptCommands.h"
#include "CRadar.h"
#include "strUtils.h"
#include "utils.h"

using namespace plugin;

CheckpointObjective::CheckpointObjective(const float* pos): BaseObjective(nullptr),
                                                            WorldObjective(nullptr),
                                                            onWhatArrive_(0),
                                                            radius_(2.0f),
                                                            textTime_(2.0f),
                                                            blipColor_(0),
                                                            indexSphere_(0),
                                                            comeBackVehicle_(boost::uuids::uuid{}),
                                                            colorBlipComeBackVehicle_(0),
                                                            sphereColor_{1.0f, 0.0f, 0.0f, 0.894f}, sphereType(1),
                                                            pulsePeriod(2048),
                                                            pulseFraction(0.1f),
                                                            rotateRate(0), blipType_(0), blipSprite_(0) {
	const auto suffix = fmt::format(" : {}", Localization::getInstance().get("objective.checkpoint"));
	strlcat(this->name_, suffix.c_str(), sizeof this->name_);
	memcpy(this->pos_, pos, sizeof this->pos_);
	createEditorBlip();
}

CheckpointObjective::CheckpointObjective(const CheckpointObjective& other): WorldObjective{other},
                                                                            IRenderable{other},
                                                                            IPositionable{other},
                                                                            onWhatArrive_{other.onWhatArrive_},
                                                                            radius_{other.radius_},
                                                                            textTime_{other.textTime_},
                                                                            blipColor_{other.blipColor_},
                                                                            indexSphere_{other.indexSphere_},
                                                                            comeBackVehicle_(other.comeBackVehicle_),
                                                                            colorBlipComeBackVehicle_{
	                                                                            other.colorBlipComeBackVehicle_
                                                                            }, sphereColor_(other.sphereColor_),
                                                                            sphereType(other.sphereType),
                                                                            pulsePeriod(other.pulsePeriod),
                                                                            pulseFraction(other.pulseFraction),
                                                                            rotateRate(other.rotateRate), blipType_(other.blipType_),
                                                                            blipSprite_(other.blipSprite_) {
	memcpy(this->pos_, other.pos_, sizeof this->pos_);
	strlcpy(this->textComeBackVehicle_, other.textComeBackVehicle_, sizeof this->textComeBackVehicle_);
	strlcpy(this->text_, other.text_, sizeof this->text_);
	createEditorBlip();
}

CheckpointObjective& CheckpointObjective::operator=(CheckpointObjective other) {
	using std::swap;
	swap(*this, other);
	return *this;
}

extern bool restart;
CheckpointObjective::~CheckpointObjective() {
	if (!restart) {
		removeEditorBlip();
		removeProjectBlip();
	}
}

void CheckpointObjective::render() {
	if (this->rerender) {
		this->rerender = false;
		return;
	}
	auto pos = CVector(this->pos_[0], this->pos_[1], this->pos_[2]);
	const auto color = floatColorToCRGBA(this->sphereColor_);
	C3dMarkers::PlaceMarkerSet(reinterpret_cast<unsigned>(this),
		static_cast<unsigned short>(this->sphereType), pos, this->radius_,
		color.r, color.g, color.b, color.a,
		static_cast<unsigned short>(this->pulsePeriod),
		this->pulseFraction,
		static_cast<short>(this->rotateRate));
}

ktwait CheckpointObjective::execute(Scene* scene) {
	auto playerPed = static_cast<CPed*>(FindPlayerPed());

	auto cp1251Text = utf8ToCp1251(this->text_);
	gxtEncode(cp1251Text);
	strlcpy(this->gameText, cp1251Text.c_str(), sizeof this->gameText);

	CMessages::AddMessage(this->gameText, static_cast<unsigned>(this->textTime_ * 1000.0f), 0, false);
	this->createProjectBlip();

	int vehicleIdx = -1;
	if (this->onWhatArrive_ == 3)
		vehicleIdx = utils::indexByUuid(scene->getVehicles(), this->comeBackVehicle_);

	bool isLocate = false;
	while (!isLocate) {
		this->render();

		switch (this->onWhatArrive_) {
		case 0:
			isLocate = Command<Commands::LOCATE_CHAR_ANY_MEANS_3D>(playerPed, this->pos_[0], this->pos_[1], this->pos_[2], this->radius_, this->radius_, this->radius_, false);
			break;
		case 1:
			isLocate = Command<Commands::LOCATE_CHAR_ON_FOOT_3D>(playerPed, this->pos_[0], this->pos_[1], this->pos_[2], this->radius_, this->radius_, this->radius_, false);
			break;
		case 2:
			isLocate = Command<Commands::LOCATE_CHAR_IN_CAR_3D>(playerPed, this->pos_[0], this->pos_[1], this->pos_[2], this->radius_, this->radius_, this->radius_, false);
			break;
		case 3: {
			auto vehicle = scene->getVehicles().at(vehicleIdx)->getProjectVehicle().value();
			bool isPlayerInCar = Command<Commands::IS_CHAR_IN_CAR>(playerPed, vehicle);

			if (!isPlayerInCar) {
				this->removeProjectBlip();

				auto cp1251TextComeBack = utf8ToCp1251(this->textComeBackVehicle_);
				gxtEncode(cp1251TextComeBack);
				CMessages::AddMessageJumpQ(const_cast<char*>(cp1251TextComeBack.c_str()), 2000u, 0, true);

				int blipComeBack;

				if (this->colorBlipComeBackVehicle_ > 0) {
					Command<Commands::ADD_BLIP_FOR_CAR>(vehicle, &blipComeBack);
					this->projectComeBackBlip = blipComeBack;
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

				CMessages::AddMessageJumpQ(this->gameText, static_cast<unsigned>(this->textTime_ * 1000.0f), 0, false);
				this->createProjectBlip();
			}

			isLocate = Command<Commands::LOCATE_CHAR_IN_CAR_3D>(playerPed, this->pos_[0], this->pos_[1], this->pos_[2], this->radius_, this->radius_, this->radius_, false);
			break;
		}
		default: 
			break;
		}

		co_await 1;
	}

	this->removeProjectComeBackBlip();
	this->removeProjectBlip();
}

void CheckpointObjective::draw(Localization& local) {
    //position
    DragPosition(this->getPosition(), [&] {
	    createEditorBlip();
    });

	ImGui::SliderInt(local.get("checkpoint_objective.type_sphere").c_str(), &this->sphereType, 0, 6);
	if (ImGui::ColorEdit4(local.get("checkpoint_objective.color_sphere").c_str(), this->sphereColor_.data())) {
		this->rerender = true;
	}
	ImGui::DragInt(local.get("checkpoint_objective.pulse_period").c_str(), &this->pulsePeriod, 64, 0, USHRT_MAX);
	ImGui::InputFloat(local.get("checkpoint_objective.pulse_fraction").c_str(), &this->pulseFraction, .0f, .0f);
	ImGui::DragInt(local.get("checkpoint_objective.rotate_rate").c_str(), &this->rotateRate, 10, SHRT_MIN, SHRT_MAX);
	if (ImGui::DragFloat(local.get("general.radius").c_str(), &this->radius_, 0.05f, 0.0f, 100.0f)) {
		this->rerender = true;
	}

	ImGui::Separator();

	if (ImGui::SliderInt(local.get("general.type_marker").c_str(), &this->blipType_, 0, 1, local.getArray("general.type_marker_enum").at(this->blipType_).c_str()))
		createEditorBlip();
	if (this->blipType_ == 0) {
		if (utils::Combo(local.get("general.color_marker").c_str(), &this->blipColor_, local.getArray("general.color_marker_enum"), 9)) {
			createEditorBlip();
		}
	} else {
		popupSpriteBlipSelector_.draw(&this->blipSprite_, [this](int i) {
			createEditorBlip();
		});
	}


	ImGui::InputText(local.get("general.text").c_str(), this->text_, sizeof this->text_);
	ImGui::InputFloat(local.get("general.time").c_str(), &this->textTime_);
	ImGui::Separator();
	utils::Combo(local.get("checkpoint_objective.how_to_arrive").c_str(), &this->onWhatArrive_, local.getArray("checkpoint_objective.how_to_arrive_enum"));

	if (this->onWhatArrive_ == 3) {
		ImGui::PushID("##oArriveOnVeh");
		const auto & vehicles = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getVehicles();

		const int index = utils::indexByUuid(vehicles, this->comeBackVehicle_);

		utils::Combo(local.get("entities.vehicle").c_str(), &this->comeBackVehicle_, index, static_cast<int>(vehicles.size()), [&vehicles] (const int i) {
			return vehicles.at(i)->getName();
		}, [&vehicles](const int i) {
			return vehicles.at(i)->getUuid();
		});
		ImGui::InputText(local.get("general.text").c_str(), this->textComeBackVehicle_, sizeof this->textComeBackVehicle_);
		utils::Combo(local.get("general.color_marker").c_str(), &this->colorBlipComeBackVehicle_, local.getArray("general.color_marker_enum"));
		ImGui::PopID();
	}


	constexpr ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
	if (ImGui::Begin("##controlOverlay", nullptr, windowFlags)) {
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 16.5f);
		ImGui::Text(local.get("info_overlay.camera_view").c_str());
		ImGui::Text(local.get("info_overlay.depend_zoom").c_str());
		ImGui::Text(local.get("info_overlay.move_element").c_str());
		ImGui::PopTextWrapPos();
	}
	ImGui::End();

	utils::controlCameraWithMove(this->getPosition());
}

char* CheckpointObjective::getText() {
    return this->text_;
}

char* CheckpointObjective::getTextComeBackVehicle() {
    return this->textComeBackVehicle_;
}

float& CheckpointObjective::getRadius() {
	return radius_;
}

float& CheckpointObjective::getTextTime() {
	return textTime_;
}

int& CheckpointObjective::getColorBlip() {
	return blipColor_;
}

int& CheckpointObjective::getIndexSphere() {
	return indexSphere_;
}

int& CheckpointObjective::getOnWhatArrive() {
	return onWhatArrive_;
}

boost::uuids::uuid& CheckpointObjective::getComeBackVehicle() {
	return comeBackVehicle_;
}

int& CheckpointObjective::getColorBlipComeBackVehicle() {
	return colorBlipComeBackVehicle_;
}

std::array<float, 4>& CheckpointObjective::getSphereColor() {
	return sphereColor_;
}

int& CheckpointObjective::getSphereType() {
	return sphereType;
}

int& CheckpointObjective::getPulsePeriod() {
	return pulsePeriod;
}

float& CheckpointObjective::getPulseFraction() {
	return pulseFraction;
}

int& CheckpointObjective::getRotateRate() {
	return rotateRate;
}

int& CheckpointObjective::getBlipType() {
	return blipType_;
}

int& CheckpointObjective::getSpriteBlip() {
	return blipSprite_;
}

std::optional<int>& CheckpointObjective::getEditorBlip() {
	return editorBlip;
}

std::optional<int>& CheckpointObjective::getProjectBlip() {
	return projectBlip;
}

std::optional<int>& CheckpointObjective::getProjectComeBackBlip() {
	return projectComeBackBlip;
}

CheckpointObjective::CheckpointObjective(): onWhatArrive_(0), radius_(0), textTime_(0), blipColor_(0), indexSphere_(0),
                                            comeBackVehicle_(),
                                            colorBlipComeBackVehicle_(0),
                                            sphereColor_(),
                                            sphereType(1), pulsePeriod(0),
                                            pulseFraction(0),
                                            rotateRate(0),
                                            blipType_(0),
                                            blipSprite_(0) {
}

void CheckpointObjective::createEditorBlip() {
	removeEditorBlip();
	if (this->blipColor_ != 0 || this->blipType_ == 1)
		this->editorBlip = utils::createBlip(this->pos_, this->blipType_, this->blipColor_, this->blipSprite_);
}

void CheckpointObjective::createProjectBlip() {
	removeProjectBlip();
	if (this->blipColor_ != 0 || this->blipType_ == 1)
		this->projectBlip = utils::createBlip(this->pos_, this->blipType_, this->blipColor_, this->blipSprite_);
}

void CheckpointObjective::removeProjectComeBackBlip() {
	if (this->projectComeBackBlip.has_value())
		Command<Commands::REMOVE_BLIP>(this->projectComeBackBlip.value());
	this->projectComeBackBlip = std::nullopt;
}

void CheckpointObjective::removeEditorBlip() {
	if (this->editorBlip.has_value()) 
		Command<Commands::REMOVE_BLIP>(this->editorBlip.value());
	this->editorBlip = std::nullopt;
}

void CheckpointObjective::removeProjectBlip() {
	if (this->projectBlip.has_value()) 
		Command<Commands::REMOVE_BLIP>(this->projectBlip.value());
	this->projectBlip = std::nullopt;
}

bool& CheckpointObjective::isRerender() {
	return rerender;
}

float* CheckpointObjective::getPosition() {
	return this->pos_;
}
