#define NOMINMAX
#define IMGUI_DEFINE_MATH_OPERATORS
#include "CutsceneObjective.h"

#include <CCamera.h>
#include <CGame.h>
#include <CHud.h>
#include <CMenuManager.h>
#include <CTheScripts.h>
#include <extensions/ScriptCommands.h>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include "CutsceneMutex.h"
#include "EditByPlayerService.h"
#include "imgui.h"
#include "imgui_stdlib.h"
#include "ProjectsService.h"
#include "Settings.h"
#include "strUtils.h"
#include "tweeny.h"
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

	const CVector pos = {this->position_[0], this->position_[1], this->position_[2]};
	CVector up = {0.f, sinf(RAD(this->xAngle_)), cosf(RAD(this->xAngle_))};
	TheCamera.Restore();
	TheCamera.SetCamPositionForFixedMode(&pos, &up);

	glm::quat q = {rotation_.real, rotation_.imag.x, rotation_.imag.y, rotation_.imag.z};
	glm::vec3 directionVector(0, 0, 1);
	directionVector = q * directionVector;
	CVector aimPos = {directionVector.x, directionVector.y, directionVector.z};
	aimPos += pos;

	if (this->getFollowType() != 0 && followEntity != nullptr && this->move == 0) {
		CVector offset(0, 0, 0);
		TheCamera.TakeControlNoEntity(&followEntity->GetPosition(), 2, 1);
	} else {
		TheCamera.TakeControlNoEntity(&aimPos, 2, 1);
	}
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

	static CQuaternion lastQ;

	const auto q = this->getRotation();
	const glm::quat quaternion(q.real, q.imag.x, q.imag.y, q.imag.z);
	const glm::vec3 eulerAngles = glm::eulerAngles(quaternion);

	//rotations
	static std::array<float, 3> eularRot = {0, 0, 0};
	if (abs(q.real - lastQ.real) > FLT_EPSILON || abs(q.imag.x - lastQ.imag.x) > FLT_EPSILON ||
		abs(q.imag.y - lastQ.imag.y) > FLT_EPSILON || abs(q.imag.z - lastQ.imag.z) > FLT_EPSILON) {
		eularRot[0] = glm::degrees(eulerAngles.x);
		eularRot[1] = glm::degrees(eulerAngles.y);
		eularRot[2] = glm::degrees(eulerAngles.z);
		lastQ = q;
	}
	DragRotations(eularRot.data(), [&] {
		this->getRotation().Set(RAD(eularRot[1]), RAD(eularRot[2]), RAD(eularRot[0]));
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
		if (ImGui::SliderInt(local.get("cutscene_objective.attach").c_str(), &this->attachType_, 0, 3,
		                     local.getArray("cutscene_objective.attach_types")[this->attachType_].c_str()))
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
		if (ImGui::SliderInt(local.get("cutscene_objective.follow").c_str(), &this->followType_, 0, 3,
		                     local.getArray("cutscene_objective.follow_types")[this->followType_].c_str())) {
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

	if (plugin::KeyPressed(VK_SHIFT)) {
		if (plugin::KeyPressed('Q')) {
			this->xAngle_ += 1.f;
			this->updateLocation();
		} else if (plugin::KeyPressed('E')) {
			this->xAngle_ += -1.f;
			this->updateLocation();
		}
	}
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
	CQuaternion targetOffsetRotation;

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
						targetPositionOffset = glm::quat(
							rotQuaternion.w, rotQuaternion.x, rotQuaternion.y,
							rotQuaternion.z) * targetPositionOffset;
						glm::quat quat(this->rotation_.real, this->rotation_.imag.x,
						               this->rotation_.imag.y,
						               this->rotation_.imag.z);
						quat = normalize(quat);
						auto anglePre = degrees(eulerAngles(quat));
						glm::quat q = normalize(glm::quat(
							rotQuaternion.w, rotQuaternion.x, rotQuaternion.y, rotQuaternion.z) * quat);
						auto angle = degrees(eulerAngles(q));
						targetOffsetRotation = {.imag = CVector(q.x, q.y, q.z), .real = q.w};
						auto ang2 = degrees(eulerAngles(glm::quat(targetOffsetRotation.real,
						                                          targetOffsetRotation.imag.x,
						                                          targetOffsetRotation.imag.y,
						                                          targetOffsetRotation.imag.z)));
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
						targetPositionOffset = glm::quat(
							rotQuaternion.w, rotQuaternion.x, rotQuaternion.y,
							rotQuaternion.z) * targetPositionOffset;
						glm::quat quat(this->rotation_.real, this->rotation_.imag.x,
						               this->rotation_.imag.y,
						               this->rotation_.imag.z);
						quat = normalize(quat);
						auto anglePre = degrees(eulerAngles(quat));
						glm::quat q = normalize(glm::quat(
							rotQuaternion.w, rotQuaternion.x, rotQuaternion.y, rotQuaternion.z) * quat);
						auto angle = degrees(eulerAngles(q));
						targetOffsetRotation = {.imag = CVector(q.x, q.y, q.z), .real = q.w};
						auto ang2 = degrees(eulerAngles(glm::quat(targetOffsetRotation.real,
						                                          targetOffsetRotation.imag.x,
						                                          targetOffsetRotation.imag.y,
						                                          targetOffsetRotation.imag.z)));
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
						targetPositionOffset = glm::quat(
							rotQuaternion.w, rotQuaternion.x, rotQuaternion.y,
							rotQuaternion.z) * targetPositionOffset;
						glm::quat quat(this->rotation_.real, this->rotation_.imag.x,
						               this->rotation_.imag.y,
						               this->rotation_.imag.z);
						quat = normalize(quat);
						auto anglePre = degrees(eulerAngles(quat));
						glm::quat q = normalize(glm::quat(
							rotQuaternion.w, rotQuaternion.x, rotQuaternion.y, rotQuaternion.z) * quat);
						auto angle = degrees(eulerAngles(q));
						targetOffsetRotation = {.imag = CVector(q.x, q.y, q.z), .real = q.w};
						auto ang2 = degrees(eulerAngles(glm::quat(targetOffsetRotation.real,
						                                          targetOffsetRotation.imag.x,
						                                          targetOffsetRotation.imag.y,
						                                          targetOffsetRotation.imag.z)));
					}
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
	                      CQuaternion targetOffsetRotation,
	                      CEntity *targetFollow, CEntity *targetAttach, bool lockPlayerControl) -> ktwait {
		CutsceneMutexGuard guard;

		if (cutscene->isStartFadeOut())
			plugin::Command<Commands::DO_FADE>(static_cast<int>(cutscene->getStartFadeOutTime() * 1000.f), 1);

		bool useEndFadeIn = false;

		const CVector pos = {cutscene->getPosition()[0], cutscene->getPosition()[1], cutscene->getPosition()[2]};
		auto endUp = RAD(cutscene->getXAngle());

		const CVector up = {0.f, sinf(RAD(cutscene->getXAngle())), cosf(RAD(cutscene->getXAngle()))};

		glm::quat q = {
			cutscene->getRotation().real, cutscene->getRotation().imag.x, cutscene->getRotation().imag.y,
			cutscene->getRotation().imag.z
		};
		//q = conjugate(q);

		if (cutscene->move == 0) {
			TheCamera.SetCamPositionForFixedMode(&pos, &up);
			if (cutscene->getAttachType() == 0 && cutscene->getFollowType() == 0) {
				glm::vec3 directionVector(0, 0, 1);
				directionVector = q * directionVector;
				CVector aimPos = {directionVector.x, directionVector.y, directionVector.z};
				aimPos += pos;
				TheCamera.TakeControlNoEntity(&aimPos, 2, 1);
			} else {
				if (targetFollow == nullptr) {
					q = {
						targetOffsetRotation.real, targetOffsetRotation.imag.x, targetOffsetRotation.imag.y,
						targetOffsetRotation.imag.z
					};
				}
				glm::vec3 directionVector(0, 0, 1);
				directionVector = q * directionVector;
				CVector aimPos = {directionVector.x, directionVector.y, directionVector.z};
				auto targetPosOffset = CVector(targetPositionOffset.x, targetPositionOffset.y, targetPositionOffset.z);
				aimPos += targetPosOffset;
				TheCamera.TakeControlAttachToEntity(targetFollow, targetAttach, &targetPosOffset, &aimPos, endUp, 2,
				                                    1);
			}
		} else {
			auto startPos = TheCamera.GetPosition();
			glm::vec3 directionVector(0, 0, 1);
			directionVector = q * directionVector;
			CVector aimPos = {directionVector.x, directionVector.y, directionVector.z};
			aimPos += pos;
			auto startPoint = startPos + CVector(TheCamera.m_mViewMatrix.right.z, TheCamera.m_mViewMatrix.up.z,
			                                     TheCamera.m_mViewMatrix.at.z);

			TheCamera.VectorTrackLinear(&aimPos, &startPoint, cutscene->getTextTime() * 1000.f, cutscene->move == 2);
			TheCamera.VectorMoveLinear(const_cast<CVector*>(&pos), &startPos, cutscene->getTextTime() * 1000.f,
			                           cutscene->move == 2);
		}

		const std::chrono::milliseconds duration(static_cast<unsigned>(cutscene->getTextTime() * 1000.f));
		if (cutscene->isEndFadeIn()) {
			const std::chrono::milliseconds fadeInDuration(static_cast<int>(cutscene->getEndFadeInTime() * 1000.f));
			co_await (duration - fadeInDuration);
			plugin::Command<Commands::DO_FADE>(static_cast<int>(fadeInDuration.count()), 0);
			co_await fadeInDuration;
		} else {
			co_await duration;
		}

		if (cutscene->isEndCutscene()) {
			CTheScripts::bDisplayHud = true;
			CHud::bScriptDontDisplayRadar = false;
			TheCamera.RestoreWithJumpCut();
		}

		if (lockPlayerControl)
			Command<Commands::SET_PLAYER_CONTROL>(0, true);

		if (cutscene->isWideScreen())
			TheCamera.SetWideScreenOff();
	};

	if (this->async_)
		tasklist.add_task(task, this, targetPositionOffset, targetOffsetRotation, targetFollow, targetAttach,
		                  lockPlayerControl);
	else
		co_await task(this, targetPositionOffset, targetOffsetRotation, targetFollow, targetAttach, lockPlayerControl);
}

void CutsceneObjective::open() {
	WorldObjective::open();
	updateLocation();
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
