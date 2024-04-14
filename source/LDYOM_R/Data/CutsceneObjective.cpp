#define NOMINMAX
#define IMGUI_DEFINE_MATH_OPERATORS
#include "CutsceneObjective.h"

#include <CCamera.h>
#include <CGame.h>
#include <CHud.h>
#include <CMenuManager.h>
#include <CStreaming.h>
#include <CTheScripts.h>
#include <extensions/ScriptCommands.h>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "CCameraExtend.h"
#include "CutsceneMutex.h"
#include "EditByPlayerService.h"
#include "imgui.h"
#include "imgui_stdlib.h"
#include "ProjectPlayerService.h"
#include "ProjectsService.h"
#include "Settings.h"
#include "strUtils.h"
#include "TimeUtils.h"
#include "utils.h"
#include "../Windows/utilsRender.h"


void CutsceneObjective::updateLocation() {
	const auto currentScene = ProjectsService::getInstance().getCurrentProject().getCurrentScene();
	CEntity *followEntity = nullptr;
	int index = -1;
	switch (this->followType_) {
		case 1:
			index = utils::indexByUuid(
				currentScene->getActors(), this->followUuid_);
			if (index != -1) {
				if (const auto ped = currentScene->getActors().at(
					index)->getEditorPed(); ped.has_value()) {
					followEntity = ped.value();
				}
			}
			break;
		case 2:
			index = utils::indexByUuid(
				currentScene->getVehicles(), this->followUuid_);
			if (index != -1) {
				if (const auto vehicle = currentScene->getVehicles().at(
					index)->getEditorVehicle(); vehicle.has_value()) {
					followEntity = vehicle.value();
				}
			}
			break;
		case 3:
			index = utils::indexByUuid(
				currentScene->getObjects(), this->followUuid_);
			if (index != -1) {
				if (const auto object = currentScene->getObjects().at(
					index)->getEditorObject(); object.has_value()) {
					followEntity = object.value();
				}
			}
			break;
		default:
			break;
	}

	TheCameraExtend.matrix.SetRotate(this->getRotation());
	TheCameraExtend.matrix.SetTranslateOnly(this->position_[0], this->position_[1], this->position_[2]);
}

CutsceneObjective::CutsceneObjective(const CVector &position, const CQuaternion &rotation): BaseObjective(nullptr),
	rotation_(rotation) {
	const auto suffix = fmt::format(" : {}", Localization::getInstance().get("objective.cutscene"));
	this->name += suffix;

	this->position_ = {position.x, position.y, position.z};
}

void CutsceneObjective::draw(Localization &local, std::vector<std::string> &listOverlay) {
	ImGui::InputText(local.get("general.text").c_str(), &this->text);
	ImGui::DragFloat(local.get("general.time").c_str(), &this->textTime_, 0.001f);

	ImGui::Separator();

	//position
	DragPosition(this->position_.data(), [&] {
		this->updateLocation();
	});

	auto q = glm::quat(this->rotation_.real, this->rotation_.imag.x, this->rotation_.imag.y, this->rotation_.imag.z);
	auto eularVec = degrees(eulerAngles(q));
	float eularArr[3] = {eularVec[0], eularVec[1], eularVec[2]};


	//rotations
	DragRotations(eularArr, [&] {
		auto q = glm::quat(radians(glm::vec3(eularArr[0], eularArr[1], eularArr[2])));
		this->getRotation() = {.imag = CVector(q.x, q.y, q.z), .real = q.w};
		this->updateLocation();
	});

	if (ImGui::Button(local.get("general.edit_manually").c_str())) {
		EditByPlayerService::getInstance().editByPlayerCamera(this->position_.data(), &this->rotation_,
		                                                      this->wideScreen_, [this] {
			                                                      this->xAngle_ = 0.f;
			                                                      this->updateLocation();
		                                                      });
	}

	ImGui::Separator();

	if (utils::Combo(local.get("cutscene_objective.move_type").c_str(), &this->move,
	                 local.getArray("cutscene_objective.move_types"))) {
		this->updateLocation();
	}


	auto entityCombo = [&]<typename T>(const char *name, boost::uuids::uuid *uuid,
	                                   std::vector<std::unique_ptr<T>> &entities) -> bool {
		const int index = utils::indexByUuid(entities, *uuid);

		bool result = false;
		IncorrectHighlight(index == -1, [&] {
			result = utils::Combo(local.get(name).c_str(), uuid, index, static_cast<int>(entities.size()),
			                      [&entities](const int i) {
				                      return std::ref(entities.at(i)->getName());
			                      }, [&entities](const int i) {
				                      return entities.at(i)->getUuid();
			                      });
		});

		if (Settings::getInstance().get<bool>("main.autoBindRequireFields").value_or(true)) {
			if (index == -1) {
				if (!entities.empty()) {
					*uuid = entities.back()->getUuid();
				}
			}
		}

		return result;
	};

	ImGui::BeginDisabled(this->move != 0);
	{
		ImGui::PushID("attachGroup");
		if (ImGui::SliderInt(local.get("cutscene_objective.attach").c_str(), &this->attachType_, 0, 4,
		                     local.getArray("cutscene_objective.entities_types")[this->attachType_].c_str()))
			this->attachUuid_ = boost::uuids::uuid{};

		bool updateLocation = false;

		switch (this->attachType_) {
			case 1:
				updateLocation = entityCombo("entities.actor", &this->attachUuid_,
				                             ProjectsService::getInstance().getCurrentProject().getCurrentScene()->
				                                                            getActors());
				break;
			case 2:
				updateLocation = entityCombo("entities.vehicle", &this->attachUuid_,
				                             ProjectsService::getInstance().getCurrentProject().getCurrentScene()->
				                                                            getVehicles());
				break;
			case 3:
				updateLocation = entityCombo("entities.object", &this->attachUuid_,
				                             ProjectsService::getInstance().getCurrentProject().getCurrentScene()->
				                                                            getObjects());
				break;
			default:
				break;
		}
		ImGui::PopID();

		ImGui::PushID("followGroup");
		if (ImGui::SliderInt(local.get("cutscene_objective.follow").c_str(), &this->followType_, 0, 4,
		                     local.getArray("cutscene_objective.entities_types")[this->followType_].c_str())) {
			this->followUuid_ = boost::uuids::uuid{};
			this->updateLocation();
		}

		switch (this->followType_) {
			case 1:
				updateLocation = entityCombo("entities.actor", &this->followUuid_,
				                             ProjectsService::getInstance().getCurrentProject().getCurrentScene()->
				                                                            getActors());
				break;
			case 2:
				updateLocation = entityCombo("entities.vehicle", &this->followUuid_,
				                             ProjectsService::getInstance().getCurrentProject().getCurrentScene()->
				                                                            getVehicles());
				break;
			case 3:
				updateLocation = entityCombo("entities.object", &this->followUuid_,
				                             ProjectsService::getInstance().getCurrentProject().getCurrentScene()->
				                                                            getObjects());
				break;
			default:
				break;
		}
		if (updateLocation)
			this->updateLocation();
		ImGui::PopID();
	}
	ImGui::EndDisabled();

	ImGui::BeginDisabled(this->attachType_ != 0);

	ImGui::Separator();

	ImGui::EndDisabled();

	ImGui::Separator();

	utils::ToggleButton(local.get("cutscene_objective.start_fade_out").c_str(), &this->startFadeOut);
	ImGui::BeginDisabled(!this->startFadeOut);
	ImGui::DragFloat(local.get("cutscene_objective.fade_out_time").c_str(), &this->startFadeOutTime, 0.01f);
	ImGui::EndDisabled();

	utils::ToggleButton(local.get("cutscene_objective.end_fade_in").c_str(), &this->endFadeIn);
	ImGui::BeginDisabled(!this->endFadeIn);
	ImGui::DragFloat(local.get("cutscene_objective.fade_in_time").c_str(), &this->endFadeInTime, 0.01f);
	ImGui::EndDisabled();

	utils::ToggleButton(local.get("cutscene_objective.widescreen").c_str(), &this->wideScreen_);
	utils::ToggleButton(local.get("cutscene_objective.final_scene").c_str(), &this->endCutscene_);
	utils::ToggleButton(local.get("cutscene_objective.lock_player_control").c_str(), &this->lockPlayerControl_);
	utils::ToggleButton(local.get("cutscene_objective.asynchronous").c_str(), &this->async_);

	listOverlay.emplace_back(local.get("info_overlay.rotate_with_shift"));
}

ktwait CutsceneObjective::execute(Scene *scene, Result &result, ktcoro_tasklist &tasklist) {
	using namespace plugin;

	while (CutsceneMutex::isLocked()) {
		co_await 1;
	}


	bool lockPlayerControl = this->lockPlayerControl_ && Command<Commands::IS_PLAYER_CONTROL_ON>(0);

	if (lockPlayerControl)
		Command<Commands::SET_PLAYER_CONTROL>(0, false);

	if (this->isWideScreen())
		TheCamera.SetWideScreenOn();

	CEntity *targetAttach = nullptr;
	int indexAttach = -1;
	glm::vec3 targetPositionOffset = {0.f, 0.f, 0.f};
	glm::quat targetOffsetRotation{};

	if (this->attachType_ > 0) {
		switch (this->attachType_) {
			case 1:
				indexAttach = utils::indexByUuid(scene->getActors(), this->attachUuid_);
				if (indexAttach != -1) {
					if (const auto ped = scene->getActors().at(indexAttach)->getProjectPed(); ped.has_value()) {
						targetAttach = ped.value();
						targetPositionOffset = {
							this->position_[0] - scene->getActors().at(indexAttach)->getPosition()[0],
							this->position_[1] - scene->getActors().at(indexAttach)->getPosition()[1],
							this->position_[2] - scene->getActors().at(indexAttach)->getPosition()[2]
						};
						glm::quat rotQuaternion = angleAxis(glm::radians(
							                                    -scene->getActors().at(indexAttach)->
							                                            getHeadingAngle()),
						                                    glm::vec3(0.0f, 0.0f, 1.0f));
						targetPositionOffset = rotQuaternion * targetPositionOffset;
						glm::quat quat(this->rotation_.real, this->rotation_.imag.x,
						               this->rotation_.imag.y,
						               this->rotation_.imag.z);
						targetOffsetRotation = rotQuaternion * quat;
					}
				}
				break;
			case 2:
				indexAttach = utils::indexByUuid(scene->getVehicles(), this->attachUuid_);
				if (indexAttach != -1) {
					if (const auto vehicle = scene->getVehicles().at(indexAttach)->getProjectVehicle(); vehicle.
						has_value()) {
						targetAttach = vehicle.value();
						targetPositionOffset = {
							this->position_[0] - scene->getVehicles().at(indexAttach)->getPosition()[0],
							this->position_[1] - scene->getVehicles().at(indexAttach)->getPosition()[1],
							this->position_[2] - scene->getVehicles().at(indexAttach)->getPosition()[2]
						};
						glm::quat rotQuaternion = angleAxis(glm::radians(
							                                    -scene->getVehicles().at(indexAttach)->
							                                            getHeadingAngle()),
						                                    glm::vec3(0.0f, 0.0f, 1.0f));
						targetPositionOffset = rotQuaternion * targetPositionOffset;
						glm::quat quat(this->rotation_.real, this->rotation_.imag.x,
						               this->rotation_.imag.y,
						               this->rotation_.imag.z);
						targetOffsetRotation = rotQuaternion * quat;
					}
				}
				break;
			case 3:
				indexAttach = utils::indexByUuid(scene->getObjects(), this->attachUuid_);
				if (indexAttach != -1) {
					auto &objectRef = scene->getObjects().at(indexAttach);
					if (const auto object = objectRef->getProjectObject(); object.
						has_value()) {
						targetAttach = object.value();
						targetPositionOffset = {
							this->position_[0] - objectRef->getPosition()[0],
							this->position_[1] - objectRef->getPosition()[1],
							this->position_[2] - objectRef->getPosition()[2]
						};
						glm::quat rotQuaternion(objectRef->getRotations().real, objectRef->getRotations().imag.x,
						                        objectRef->getRotations().imag.y, objectRef->getRotations().imag.z);
						targetPositionOffset = rotQuaternion * targetPositionOffset;
						glm::quat quat(this->rotation_.real, this->rotation_.imag.x,
						               this->rotation_.imag.y,
						               this->rotation_.imag.z);
						targetOffsetRotation = rotQuaternion * quat;
					}
				}
				break;
			case 4:
				if (const auto ped = FindPlayerPed(); ped) {
					indexAttach = 0;
					targetAttach = ped;
					auto playerCoors = FindPlayerCoors(0);
					targetPositionOffset = {
						this->position_[0] - playerCoors.x,
						this->position_[1] - playerCoors.y,
						this->position_[2] - playerCoors.z
					};
					glm::quat rotQuaternion = angleAxis(-FindPlayerHeading(0),
					                                    glm::vec3(0.0f, 0.0f, 1.0f));
					targetPositionOffset = rotQuaternion * targetPositionOffset;
					glm::quat quat(this->rotation_.real, this->rotation_.imag.x,
					               this->rotation_.imag.y,
					               this->rotation_.imag.z);
					targetOffsetRotation = rotQuaternion * quat;
				}
				break;
			default:
				break;
		}

		if (indexAttach == -1) {
			setObjectiveError(result, *this, NotSelected, "The attach entity for the objective is not selected.");
			co_return;
		}

		if (targetAttach == nullptr) {
			setObjectiveError(result, *this, NotExists, "The attach entity does not exist.");
			co_return;
		}
	}

	CEntity *targetFollow = nullptr;
	int indexFollow = -1;

	if (this->followType_ > 0) {
		switch (this->followType_) {
			case 1:
				indexFollow = utils::indexByUuid(scene->getActors(), this->followUuid_);
				if (indexFollow != -1) {
					if (const auto ped = scene->getActors().at(indexFollow)->getProjectPed(); ped.has_value()) {
						targetFollow = ped.value();
					}
				}
				break;
			case 2:
				indexFollow = utils::indexByUuid(scene->getVehicles(), this->followUuid_);
				if (indexFollow != -1) {
					if (const auto vehicle = scene->getVehicles().at(indexFollow)->getProjectVehicle(); vehicle.
						has_value())
						targetFollow = vehicle.value();
				}
				break;
			case 3:
				indexFollow = utils::indexByUuid(scene->getObjects(), this->followUuid_);
				if (indexFollow != -1) {
					if (const auto object = scene->getObjects().at(indexFollow)->getProjectObject(); object.has_value())
						targetFollow = object.value();
				}
				break;
			case 4:
				indexFollow = 0;
				targetFollow = FindPlayerPed();
				break;
			default:
				break;
		}

		if (indexFollow == -1) {
			setObjectiveError(result, *this, NotSelected, "The follow entity for the objective is not selected.");
			co_return;
		}

		if (targetFollow == nullptr) {
			setObjectiveError(result, *this, NotExists, "The follow entity does not exist.");
			co_return;
		}
	}

	auto cp1251Text = utf8ToCp1251(this->text);
	gxtEncode(cp1251Text);
	this->gameText = cp1251Text;

	CMessages::AddMessage(this->gameText.data(), static_cast<unsigned>(this->textTime_ * 1000.0f), 0, false);

	CTheScripts::bDisplayHud = false;
	CHud::bScriptDontDisplayRadar = true;

	static auto task = [](CutsceneObjective *cutscene, glm::vec3 targetPositionOffset,
	                      glm::quat targetOffsetRotation,
	                      CEntity *targetFollow, CEntity *targetAttach, bool lockPlayerControl) -> ktwait {
		CutsceneMutexGuard guard;

		if (cutscene->isStartFadeOut())
			plugin::Command<Commands::DO_FADE>(static_cast<int>(cutscene->getStartFadeOutTime() * 1000.f), 1);

		const CVector pos = {cutscene->getPosition()[0], cutscene->getPosition()[1], cutscene->getPosition()[2]};

		TheCameraExtend.setExtendMode(true);

		if (cutscene->move == 0) {
			if (cutscene->getAttachType() == 0 && cutscene->getFollowType() == 0) {
				TheCameraExtend.matrix.SetRotate(cutscene->getRotation());
				TheCameraExtend.matrix.SetTranslateOnly(pos.x, pos.y, pos.z);
			} else {
				const CQuaternion rotationOffset = {
					{targetOffsetRotation.x, targetOffsetRotation.y, targetOffsetRotation.z}, targetOffsetRotation.w
				};
				const CVector positionOffset = {targetPositionOffset.x, targetPositionOffset.y, targetPositionOffset.z};
				TheCameraExtend.attachToEntity(targetAttach, targetFollow, rotationOffset, positionOffset);
			}
		} else {
			auto startPos = TheCamera.GetPosition();
			glm::vec3 directionVector(0, 0, 1);
			directionVector = glm::quat(cutscene->rotation_.real, cutscene->rotation_.imag.x,
			                            cutscene->rotation_.imag.y,
			                            cutscene->rotation_.imag.z) * directionVector;
			CVector aimPos = {directionVector.x, directionVector.y, directionVector.z};
			aimPos += pos;
			auto startPoint = startPos + TheCameraExtend.matrix.at;

			TheCamera.VectorTrackLinear(&aimPos, &startPoint, cutscene->getTextTime() * 1000.f, cutscene->move == 2);
			TheCamera.VectorMoveLinear(const_cast<CVector*>(&pos), &startPos, cutscene->getTextTime() * 1000.f,
			                           cutscene->move == 2);
		}

		const std::chrono::milliseconds duration(static_cast<unsigned>(cutscene->getTextTime() * 1000.f));
		if (cutscene->isEndFadeIn()) {
			const std::chrono::milliseconds fadeInDuration(static_cast<int>(cutscene->getEndFadeInTime() * 1000.f));
			co_await waitInGame((duration - fadeInDuration).count());
			plugin::Command<Commands::DO_FADE>(static_cast<int>(fadeInDuration.count()), 0);
			co_await waitInGame(fadeInDuration.count());
		} else {
			co_await waitInGame(duration.count());
		}

		const CVector up = {0.0f, 0.0f, 1.0f};
		TheCamera.SetCamPositionForFixedMode(&TheCameraExtend.matrix.pos, &up);
		const CVector fixedVector = TheCameraExtend.matrix.pos + TheCameraExtend.matrix.at;
		TheCamera.TakeControlNoEntity(&fixedVector, 2, 1);
		TheCameraExtend.setExtendMode(false);

		if (cutscene->isEndCutscene()) {
			CTheScripts::bDisplayHud = true;
			CHud::bScriptDontDisplayRadar = false;
			TheCamera.RestoreWithJumpCut();
		}

		if (lockPlayerControl)
			Command<Commands::SET_PLAYER_CONTROL>(0, true);

		if (cutscene->isWideScreen())
			TheCamera.SetWideScreenOff();

		if (cutscene->attachFreeCamera.has_value()) {
			if (const auto object = cutscene->attachFreeCamera.value(); CPools::ms_pObjectPool->IsObjectValid(object)) {
				const int objectRef = CPools::GetObjectRef(object);
				Command<Commands::DELETE_OBJECT>(objectRef);
			}
			cutscene->attachFreeCamera = std::nullopt;
		}
	};

	if (this->async_)
		tasklist.add_task(task, this, targetPositionOffset, targetOffsetRotation, targetFollow, targetAttach,
		                  lockPlayerControl);
	else
		co_await task(this, targetPositionOffset, targetOffsetRotation, targetFollow, targetAttach, lockPlayerControl);
}

void CutsceneObjective::open() {
	WorldObjective::open();
	TheCameraExtend.setExtendMode(true);
	updateLocation();
}

void CutsceneObjective::close() {
	TheCameraExtend.setExtendMode(false);
	WorldObjective::close();
}

int& CutsceneObjective::getAttachType() {
	return attachType_;
}

boost::uuids::uuid& CutsceneObjective::getAttachUuid() {
	return attachUuid_;
}

int& CutsceneObjective::getFollowType() {
	return followType_;
}

boost::uuids::uuid& CutsceneObjective::getFollowUuid() {
	return followUuid_;
}

std::array<float, 3>& CutsceneObjective::getPosition() {
	return position_;
}

CQuaternion& CutsceneObjective::getRotation() {
	return rotation_;
}

float& CutsceneObjective::getXAngle() {
	return xAngle_;
}

std::string& CutsceneObjective::getText() {
	return text;
}

float& CutsceneObjective::getTextTime() {
	return textTime_;
}

bool& CutsceneObjective::isWideScreen() {
	return wideScreen_;
}

bool& CutsceneObjective::isAsync() {
	return async_;
}

bool& CutsceneObjective::isStartFadeOut() {
	return startFadeOut;
}

float& CutsceneObjective::getStartFadeOutTime() {
	return startFadeOutTime;
}

bool& CutsceneObjective::isEndFadeIn() {
	return endFadeIn;
}

float& CutsceneObjective::getEndFadeInTime() {
	return endFadeInTime;
}

bool& CutsceneObjective::isEndCutscene() {
	return endCutscene_;
}

std::string& CutsceneObjective::getGameText() {
	return gameText;
}

int& CutsceneObjective::getMove() { return move; }

bool& CutsceneObjective::isLockPlayerControl() { return lockPlayerControl_; }
