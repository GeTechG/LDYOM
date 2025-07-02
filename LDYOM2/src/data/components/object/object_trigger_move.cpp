#include "object_trigger_move.h"
#include "components/object.h"
#include "entity.h"
#include "extensions/ScriptCommands.h"
#include <CTheScripts.h>
#include <angles.h>
#include <common.h>
#include <components/object.h>
#include <cquat_json.h>
#include <glm/ext/quaternion_float.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/trigonometric.hpp>
#include <in_game/object_transform_editing.h>
#include <matrix_utils.h>
#include <project_player.h>
#include <window_manager.h>

void components::ObjectTriggerMove::sol_lua_register(sol::state_view lua_state) {
	auto ut = lua_state.new_usertype<ObjectTriggerMove>("ObjectTriggerMoveComponent");
	SOL_LUA_FOR_EACH(SOL_LUA_BIND_MEMBER_ACTION, ut, components::ObjectTriggerMove, cast, radius, time, endPosition,
	                 endRotation);
}

components::ObjectTriggerMove::ObjectTriggerMove()
	: Component(TYPE) {}

nlohmann::json components::ObjectTriggerMove::to_json() const {
	auto j = this->Component::to_json();
	j["radius"] = radius;
	j["time"] = time;
	j["endPosition"] = endPosition;
	j["endRotation"] = endRotation;
	return j;
}

void components::ObjectTriggerMove::from_json(const nlohmann::json& j) {
	this->Component::from_json(j);
	j.at("radius").get_to(radius);
	j.at("time").get_to(time);
	j.at("endPosition").get_to(endPosition);
	j.at("endRotation").get_to(endRotation);
}

void components::ObjectTriggerMove::editorRender() {
	const auto availableWidth = ImGui::GetContentRegionAvail().x;

	ImGui::Text(tr("radius").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	if (ImGui::InputInt("##radius", &radius)) {
		this->dirtySphere = true;
	}

	ImGui::Text(tr("time").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::InputFloat("##time", &time);

	ImGui::Text(tr("end_position").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::InputFloat3("##endPosition", endPosition.data());

	static CQuaternion lastQ;
	const glm::quat quaternion(endRotation.real, endRotation.imag.x, endRotation.imag.y, endRotation.imag.z);
	const glm::vec3 eulerAngles = glm::eulerAngles(quaternion);

	// rotations
	static std::array<float, 3> eularRot = {0, 0, 0};
	if (abs(endRotation.real - lastQ.real) > FLT_EPSILON || abs(endRotation.imag.x - lastQ.imag.x) > FLT_EPSILON ||
	    abs(endRotation.imag.y - lastQ.imag.y) > FLT_EPSILON || abs(endRotation.imag.z - lastQ.imag.z) > FLT_EPSILON) {
		eularRot[0] = glm::degrees(eulerAngles.x);
		eularRot[1] = glm::degrees(eulerAngles.y);
		eularRot[2] = glm::degrees(eulerAngles.z);
		lastQ = endRotation;
	}

	ImGui::Text(tr("end_rotation").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	if (ImGui::InputFloat3("##endRotation", eularRot.data())) {
		endRotation.Set(RAD(eularRot[1]), RAD(eularRot[2]), RAD(eularRot[0]));
	}

	if (ImGui::Button(tr("edit").c_str())) {
		WindowManager::instance().disableWindowRendering(true);
		auto object = Object::cast(this->entity->getComponent(Object::TYPE));
		if (object && object->handle) {
			ObjectTransformEditing::openTransformEditing(&*object->handle, this->endPosition, this->endRotation,
			                                             this->entity->scale,
			                                             [this](ObjectTransformEditingCallbackData data) {
															 if (data.saveChanges) {
																 this->endPosition = data.position;
																 this->endRotation = data.rotation;
															 }
															 this->entity->reset();
															 WindowManager::instance().disableWindowRendering(false);
														 });
		}
	}
}

void components::ObjectTriggerMove::onStart() {
	Component::onStart();
	if (IS_PLAYING) {
		this->progress = 0.f;
	} else {
		if (this->endPosition[0] == 0.f && this->endPosition[1] == 0.f && this->endPosition[2] == 0.f) {
			this->endPosition = {this->entity->position[0], this->entity->position[1], this->entity->position[2]};
		}
		if (this->sphereHandle) {
			plugin::Command<plugin::Commands::REMOVE_SPHERE>(this->sphereHandle.value());
			this->sphereHandle.reset();
		}
		this->dirtySphere = true;
	}
}

void components::ObjectTriggerMove::onUpdate(float deltaTime) {
	Component::onUpdate(deltaTime);
	if (IS_PLAYING) {
		const auto playerPosition = FindPlayerPed()->GetPosition();
		const auto object = Object::cast(this->entity->getComponent(Object::TYPE));
		const CVector endPosition(this->endPosition[0], this->endPosition[1], this->endPosition[2]);
		if (object && object->handle) {
			const auto objectPosition =
				CVector(this->entity->position[0], this->entity->position[1], this->entity->position[2]);
			const float distance = DistanceBetweenPoints(playerPosition, objectPosition);
			bool isMoving = true;
			if (distance <= radius) {
				this->progress += deltaTime;
				if (this->progress >= time) {
					this->progress = time;
				}
			} else {
				this->progress -= deltaTime;
				if (this->progress < 0.f) {
					this->progress = 0.f;
				}
			}
			if (isMoving) {
				const float t = this->progress / time;
				const auto newPosition = objectPosition + (endPosition - objectPosition) * t;
				CQuaternion newRotation;
				newRotation.Slerp(this->entity->rotation, this->endRotation, t);
				object->handle->SetPosn(newPosition.x, newPosition.y, newPosition.z);
				object->handle->m_matrix->SetRotate(newRotation);
				scaleMatrix(*object->handle->m_matrix, this->entity->scale);
				object->handle->m_matrix->UpdateRW();
				object->handle->UpdateRwMatrix();
				object->handle->UpdateRwFrame();
			}
		}
	} else {
		if (this->dirtySphere) {
			if (this->sphereHandle) {
				plugin::Command<plugin::Commands::REMOVE_SPHERE>(this->sphereHandle.value());
				this->sphereHandle.reset();
			}
			int newSphereHandle;
			plugin::Command<plugin::Commands::ADD_SPHERE>(this->entity->position[0], this->entity->position[1],
			                                              this->entity->position[2], (float)radius, &newSphereHandle);
			if (newSphereHandle != -1) {
				this->sphereHandle = newSphereHandle;
			}
			this->dirtySphere = false;
		}
		if (this->sphereHandle) {
			auto actualIndex = CTheScripts::GetActualScriptThingIndex(this->sphereHandle.value(), 0);
			if (actualIndex != -1) {
				CTheScripts::ScriptSphereArray[actualIndex].vCoords =
					RwV3d(this->entity->position[0], this->entity->position[1], this->entity->position[2]);
			}
		}
	}
}

void components::ObjectTriggerMove::onReset() {
	Component::onReset();
	if (this->sphereHandle) {
		plugin::Command<plugin::Commands::REMOVE_SPHERE>(this->sphereHandle.value());
		this->sphereHandle.reset();
	}
}

Dependencies components::ObjectTriggerMove::getDependencies() { return Dependencies{{Object::TYPE}, true}; }