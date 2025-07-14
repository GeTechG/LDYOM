#define IMGUI_DEFINE_MATH_OPERATORS
#include "checkpoint.h"
#include "extensions/ScriptCommands.h"
#include "objective_specific.h"
#include <CFireManager.h>
#include <CStreaming.h>
#include <CTheScripts.h>
#include <CWorld.h>
#include <angles.h>
#include <corecrt_math_defines.h>
#include <entity.h>
#include <glm/ext/quaternion_float.hpp>
#include <glm/gtc/quaternion.hpp>
#include <lua_define_type.h>
#include <matrix_utils.h>
#include <popups/vehicle_selector.h>
#include <project_player.h>
#include <scenes_manager.h>
#include <string_utils.h>
#include <utils/imgui_configurate.h>

void components::Checkpoint::sol_lua_register(sol::state_view lua_state) {
	auto ut = lua_state.new_usertype<Checkpoint>("CheckpointComponent");
	SOL_LUA_FOR_EACH(SOL_LUA_BIND_MEMBER_ACTION, ut, components::Checkpoint, cast, typeCheckpoint, radius);
}

components::Checkpoint::Checkpoint()
	: Component(TYPE) {}

inline nlohmann::json components::Checkpoint::to_json() const {
	auto j = this->Component::to_json();
	j["typeCheckpoint"] = typeCheckpoint;
	j["radius"] = radius;
	return j;
}

void components::Checkpoint::from_json(const nlohmann::json& j) {
	this->Component::from_json(j);
	j.at("typeCheckpoint").get_to(typeCheckpoint);
	j.at("radius").get_to(radius);
}

void components::Checkpoint::editorRender() {
	const auto availableWidth = ImGui::GetContentRegionAvail().x;

	ImGui::Text(_("type").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	int typeCheckpoint = this->typeCheckpoint;
	if (ImGui::SliderInt("##typeCheckpoint", &typeCheckpoint, 0, 5,
	                     tr(fmt::format("types.{}", typeCheckpoint)).c_str())) {
		this->despawn();
		this->typeCheckpoint = typeCheckpoint;
		this->dirty = true;
	}

	ImGui::Text(_("radius").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	if (ImGui::DragFloat("##radius", &radius, 0.1f, 0.0f, 100.0f)) {
		this->dirty = true;
	}
}

void components::Checkpoint::onStart() {
	Component::onStart();
	this->entity->setGetTransformCallbacks([this]() -> std::array<float, 3> { return {0.0f, 0.0f, 0.0f}; },
	                                       [this]() -> CQuaternion {
											   // if (this->vehicle) {
		                                       // 	return (std::array<float,3>)&this->vehicle->GetMatrix()->rot;
		                                       // }
											   return {};
										   },
	                                       [this]() -> std::array<float, 3> {
											   // if (this->vehicle) {
		                                       // 	return (std::array<float,3>)&this->vehicle->GetMatrix()->scale;
		                                       // }
											   return {0.0f, 0.0f, 0.0f};
										   });
	this->entity->setSetTransformCallbacks(
		[this](const std::array<float, 3>& position) {
			if (this->handle) {
				if (this->typeCheckpoint == 0 || this->typeCheckpoint == 5) { // Sphere
					this->dirty = true;
				} else { // Tube Arrow, Tube End, Tube, Torus
					plugin::Command<plugin::Commands::SET_CHECKPOINT_COORDS>(this->handle.value(), position[0],
				                                                             position[1], position[2]);
				}
			}
		},
		[this](const CQuaternion rotation) {
			if (this->handle) {
				if (!(this->typeCheckpoint == 0 || this->typeCheckpoint == 5)) { // Sphere
					const glm::quat quaternion(rotation.real, rotation.imag.x, rotation.imag.y, rotation.imag.z);
					const glm::vec3 eulerAngles = glm::eulerAngles(quaternion);
					plugin::Command<plugin::Commands::SET_CHECKPOINT_HEADING>(this->handle.value(), DEG(eulerAngles.z));
				}
			}
		},
		[this](const std::array<float, 3>& scale) {
			// if (this->handle) {
		    // 	scaleMatrix(*this->handle->m_matrix, scale);
		    // 	this->handle->m_matrix->UpdateRW();
		    // 	this->handle->UpdateRwMatrix();
		    // 	this->handle->UpdateRwFrame();
		    // }
		});
	if (!IS_PLAYING) {
		spawn();
	} else {
		auto objectiveSpecific = ObjectiveSpecific::cast(this->entity->getComponent(ObjectiveSpecific::TYPE));
		if (objectiveSpecific) {
			this->onSpawnedConnection = std::make_optional(objectiveSpecific->onSpawned.connect([this]() { spawn(); }));
			this->onDespawnedConnection =
				std::make_optional(objectiveSpecific->onDespawned.connect([this]() { despawn(); }));
		}
	}
}

void components::Checkpoint::onUpdate(float deltaTime) {
	Component::onUpdate(deltaTime);
	if (this->dirty) {
		spawn();
	}
	this->dirty = false;
}

void components::Checkpoint::onReset() {
	Component::onReset();
	despawn();
	this->onSpawnedConnection.reset();
	this->onDespawnedConnection.reset();
}

void components::Checkpoint::updateRotation() {
	if (this->handle) {
		if (!(this->typeCheckpoint == 0 || this->typeCheckpoint == 5)) { // Sphere
			const auto q = entity->rotation;
			const glm::quat quaternion(q.real, q.imag.x, q.imag.y, q.imag.z);
			const glm::vec3 eulerAngles = glm::eulerAngles(quaternion);
			plugin::Command<plugin::Commands::SET_CHECKPOINT_HEADING>(this->handle.value(), eulerAngles.z);
		}
	}
}

void components::Checkpoint::updatePosition() {}

void components::Checkpoint::spawn() {
	despawn();

	int handle = 0;
	if (typeCheckpoint == 0 || typeCheckpoint == 5) { // Sphere
		plugin::Command<plugin::Commands::ADD_SPHERE>(this->entity->position[0], this->entity->position[1],
		                                              this->entity->position[2], radius, &handle);
	} else { // Tube Arrow, Tube End, Tube, Torus
		auto type = typeCheckpoint != 4 ? this->typeCheckpoint - 1 : 4;
		plugin::Command<plugin::Commands::CREATE_CHECKPOINT>(
			type, this->entity->position[0], this->entity->position[1], this->entity->position[2],
			this->entity->position[0], this->entity->position[1], this->entity->position[2], radius, &handle);
	}
	updateRotation();

	if (handle != 0) {
		this->handle = handle;
	}

	onSpawned();
}

void components::Checkpoint::despawn() {
	if (this->handle) {
		if (this->typeCheckpoint == 0 || this->typeCheckpoint == 5) { // Sphere
			plugin::Command<plugin::Commands::REMOVE_SPHERE>(this->handle.value());
		} else {
			plugin::Command<plugin::Commands::DELETE_CHECKPOINT>(this->handle.value());
		}
		this->handle.reset();
	}

	onDespawned();
}