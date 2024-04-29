#include "AttachPedToEntity.h"

#include <CCamera.h>
#include <CModelInfo.h>
#include <CStreaming.h>
#include <CWorld.h>
#include <extensions/ScriptCommands.h>
#include <extensions/scripting/ScriptCommandNames.h>

#include "CCameraExtend.h"
#include "CMatrixUtils.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_internal.h"
#include "ModelsService.h"
#include "PopupWeaponSelector.h"
#include "ProjectsService.h"
#include "Settings.h"
#include "utils.h"
#include "../Windows/utilsRender.h"
#include "glm/gtc/quaternion.hpp"

AttachPedToEntity::AttachPedToEntity(const CVector &position) : BaseObjective(nullptr) {
	const auto suffix = fmt::format(" : {}", Localization::getInstance().get("objective.attach_ped_to_entity"));
	this->name += suffix;
	this->position = {position.x, position.y, position.z};
}

extern float speedCameraMultiplier;

void AttachPedToEntity::open() {
	using namespace plugin;
	WorldObjective::open();
	speedCameraMultiplier = Settings::getInstance().get<float>("camera.editByPlayerSpeed").value_or(1.f);
	TheCameraExtend.setExtendMode(true);
	Command<Commands::FREEZE_CHAR_POSITION_AND_DONT_LOAD_COLLISION>(FindPlayerPed(), true);
	CWorld::Remove(FindPlayerPed());
}

void AttachPedToEntity::close() {
	using namespace plugin;
	WorldObjective::close();
	TheCameraExtend.setExtendMode(false);
	Command<Commands::FREEZE_CHAR_POSITION_AND_DONT_LOAD_COLLISION>(FindPlayerPed(), false);
	CWorld::Add(FindPlayerPed());
	TheCamera.Restore();
}

void AttachPedToEntity::draw(Localization &local, std::vector<std::string> &listOverlay) {
	PopupWeaponSelector::getInstance().weaponButton(&this->weaponType);

	PopupWeaponSelector::getInstance().renderPopup([this](const int weaponId) {
		this->weaponType = weaponId;
	});

	ImGui::SliderInt(local.get("attach_ped_to_entity.position_type").c_str(), &this->positionType, 0, 3,
	                 local.getArray("attach_ped_to_entity.position_types")[this->positionType].c_str());

	if (ImGui::SliderInt(local.get("attach_ped_to_entity.attach_type").c_str(), &this->attachType, 0, 1,
	                     local.getArray("attach_ped_to_entity.attach_types")[this->attachType].c_str())) {
		this->entityAttachUuid_ = {};
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

	const auto scene = ProjectsService::getInstance().getCurrentProject().getCurrentScene();

	if (this->attachType == 1) {
		entityCombo("entities.actor", &this->entityAttachUuid_, scene->getActors());
	}

	if (ImGui::SliderInt(local.get("attach_ped_to_entity.target_attach").c_str(), &this->targetType, 0, 1,
	                     local.getArray("attach_ped_to_entity.target_attach_types")[this->targetType].c_str())) {
		this->entityTargetUuid_ = {};
	}

	ImGui::SliderAngle(local.get("attach_ped_to_entity.limit_angle_x").c_str(), &this->limitAngleX, 0.f, 180.f);

	switch (this->targetType) {
		case 0: {
			entityCombo("entities.vehicle", &this->entityTargetUuid_, scene->getVehicles());
			const auto indexVehicle = utils::indexByUuid(scene->getVehicles(), this->entityTargetUuid_);
			if (indexVehicle != -1) {
				if (reinterpret_cast<CVehicleModelInfo*>(CModelInfo::GetModelInfo(
					scene->getVehicles().at(indexVehicle)->getModelId()))->m_nVehicleType == VEHICLE_BIKE) {
					ImGui::SliderAngle(local.get("attach_ped_to_entity.limit_angle_y").c_str(), &this->limitAngleY, 0.f,
					                   180.f);
				}
			}
			break;
		}
		case 1: {
			entityCombo("entities.object", &this->entityTargetUuid_, scene->getObjects());
			break;
		}
		default:
			break;
	}

	if (this->attachType == 0) {
		TheCameraExtend.setExtendMode(true);
		if (const bool isWindow = ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) || ImGui::IsAnyItemHovered();
			!isWindow) {
			const float mouseSensitive = CCamera::m_fMouseAccelHorzntal * 1000;
			if (ImGui::IsMouseDragging(ImGuiMouseButton_Right)) {
				const ImVec2 dt = ImGui::GetIO().MouseDelta * mouseSensitive * 0.2f;
				auto eular = eulerAngles(quat_cast(CMatrixToGlmMat4(TheCameraExtend.matrix)));
				eular.y += RAD(dt.y);
				eular.z += RAD(-dt.x);
				auto quat = glm::quat(eular);
				TheCameraExtend.matrix.SetRotate({{quat.x, quat.y, quat.z}, quat.w});
			}
			if (ImGui::IsMouseDown(ImGuiMouseButton_Right)) {
				CVector pos = {this->position.at(0), this->position.at(1), this->position.at(2)};
				if (ImGui::IsKeyDown(ImGuiKey_W)) {
					pos += TheCameraExtend.matrix.right * speedCameraMultiplier;
				} else if (ImGui::IsKeyDown(ImGuiKey_S)) {
					pos += TheCameraExtend.matrix.right * speedCameraMultiplier * -1;
				}

				if (ImGui::IsKeyDown(ImGuiKey_A)) {
					pos += TheCameraExtend.matrix.up * speedCameraMultiplier;
				} else if (ImGui::IsKeyDown(ImGuiKey_D)) {
					pos += TheCameraExtend.matrix.up * speedCameraMultiplier * -1;
				}

				if (ImGui::IsKeyDown(ImGuiKey_Q)) {
					pos += TheCameraExtend.matrix.at * speedCameraMultiplier;
				} else if (ImGui::IsKeyDown(ImGuiKey_E)) {
					pos += TheCameraExtend.matrix.at * speedCameraMultiplier * -1;
				}

				this->position = {pos.x, pos.y, pos.z};

				if (ImGui::GetIO().MouseWheel < 0.f) {
					speedCameraMultiplier -= 0.01f;
					speedCameraMultiplier = std::max(speedCameraMultiplier, 0.f);
					Settings::getInstance().set("camera.editByPlayerSpeed", speedCameraMultiplier);
					Settings::getInstance().Save();
				} else if (ImGui::GetIO().MouseWheel > 0.f) {
					speedCameraMultiplier += 0.01f;
					Settings::getInstance().set("camera.editByPlayerSpeed", speedCameraMultiplier);
					Settings::getInstance().Save();
				}
			}
		}
		TheCameraExtend.matrix.pos = {this->position.at(0), this->position.at(1), this->position.at(2)};
	} else if (this->attachType == 1) {
		const auto &actors = scene->getActors();
		const auto indexActor = utils::indexByUuid(scene->getActors(), this->entityAttachUuid_);
		if (indexActor != -1) {
			TheCameraExtend.setExtendMode(false);
			listOverlay.emplace_back(local.get("info_overlay.camera_view"));
			listOverlay.emplace_back(local.get("info_overlay.depend_zoom"));

			auto position = actors.at(indexActor)->getPosition();
			utils::controlCamera({position[0], position[1], position[2]});
		}
	}

	std::array<char, 500> speedMove;
	ImFormatString(speedMove.data(), sizeof speedMove, local.get("camera_paths.control").c_str(),
	               speedCameraMultiplier);
	listOverlay.emplace_back(speedMove.data());
}

ktwait AttachPedToEntity::execute(Scene *scene, Result &result, ktcoro_tasklist &tasklist) {
	CEntity *targetEntity = nullptr;
	float *targetPosition = nullptr;
	glm::quat targetRotation;
	bool isBike = false;

	switch (this->targetType) {
		case 0: {
			const auto &vehicles = scene->getVehicles();
			const auto indexTarget = utils::indexByUuid(vehicles, this->entityTargetUuid_);
			if (indexTarget != -1) {
				targetPosition = vehicles.at(indexTarget)->getPosition();
				targetRotation = angleAxis(glm::radians(-vehicles.at(indexTarget)->getHeadingAngle()),
				                           glm::vec3(0.0f, 0.0f, 1.0f));
				targetEntity = vehicles.at(indexTarget)->getProjectVehicle().value_or(nullptr);
				if (reinterpret_cast<CVehicleModelInfo*>(CModelInfo::GetModelInfo(
					vehicles.at(indexTarget)->getModelId()))->m_nVehicleType == VEHICLE_BIKE) {
					isBike = true;
				}
			}
			break;
		}
		case 1: {
			const auto &objects = scene->getObjects();
			const auto indexTarget = utils::indexByUuid(objects, this->entityTargetUuid_);
			if (indexTarget != -1) {
				targetPosition = objects.at(indexTarget)->getPosition();
				auto rot = objects.at(indexTarget)->getRotations();
				targetRotation = {rot.real, rot.imag.x, rot.imag.y, rot.imag.z};
				targetRotation = conjugate(targetRotation);
				targetEntity = objects.at(indexTarget)->getProjectObject().value_or(nullptr);
			}
			break;
		}
	}

	if (targetEntity == nullptr) {
		setObjectiveError(result, *this, NotExists, "The entity does not exist.");
		co_return;
	}

	CPed *targetAttach = nullptr;
	glm::vec3 targetPositionOffset = {0.f, 0.f, 0.f};

	if (this->attachType == 0) {
		targetAttach = FindPlayerPed();
		targetPositionOffset = {
			this->position[0] - targetPosition[0],
			this->position[1] - targetPosition[1],
			this->position[2] - targetPosition[2]
		};
		targetPositionOffset = targetRotation * targetPositionOffset;
	} else if (this->attachType == 1) {
		const auto &actors = scene->getActors();
		const auto indexAttach = utils::indexByUuid(actors, this->entityAttachUuid_);
		if (indexAttach != -1) {
			targetAttach = actors.at(indexAttach)->getProjectPed().value();
			targetPositionOffset = {
				actors.at(indexAttach)->getPosition()[0] - targetPosition[0],
				actors.at(indexAttach)->getPosition()[1] - targetPosition[1],
				actors.at(indexAttach)->getPosition()[2] - targetPosition[2]
			};
			targetPositionOffset = targetRotation * targetPositionOffset;
		}
	}

	if (targetAttach == nullptr) {
		setObjectiveError(result, *this, NotExists, "The ped does not exist.");
		co_return;
	}

	int weapon = ModelsService::validWeaponModel(this->weaponType);
	const int weaponModel = CWeaponInfo::GetWeaponInfo(static_cast<eWeaponType>(weapon), 1)->m_nModelId1;

	CStreaming::RequestModel(weaponModel, GAME_REQUIRED);
	CStreaming::LoadAllRequestedModels(false);

	while (CStreaming::ms_aInfoForModel[weaponModel].m_nLoadState != LOADSTATE_LOADED) {
		co_await 1;
	}

	if (isBike) {
		targetAttach->AttachPedToBike(targetEntity, CVector(targetPositionOffset.x, targetPositionOffset.y,
		                                                    targetPositionOffset.z), this->positionType,
		                              this->limitAngleX, this->limitAngleY, static_cast<eWeaponType>(weapon));
	} else {
		targetAttach->AttachPedToEntity(targetEntity,
		                                CVector(targetPositionOffset.x, targetPositionOffset.y, targetPositionOffset.z),
		                                static_cast<unsigned short>(this->positionType), this->limitAngleX,
		                                static_cast<eWeaponType>(weapon));
	}

	CStreaming::SetMissionDoesntRequireModel(weaponModel);
}

boost::uuids::uuid& AttachPedToEntity::getEntityAttachUuid() { return entityAttachUuid_; }
boost::uuids::uuid& AttachPedToEntity::getEntityTargetUuid() { return entityTargetUuid_; }
std::array<float, 3>& AttachPedToEntity::getPosition() { return position; }
int& AttachPedToEntity::getAttachType() { return attachType; }
int& AttachPedToEntity::getTargetType() { return targetType; }
int& AttachPedToEntity::getPositionType() { return positionType; }
float& AttachPedToEntity::getLimitAngleX() { return limitAngleX; }
float& AttachPedToEntity::getLimitAngleY() { return limitAngleY; }
int& AttachPedToEntity::getWeaponType() { return weaponType; }
