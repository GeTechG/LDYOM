#define IMGUI_DEFINE_MATH_OPERATORS
#include "firework.h"
#include "extensions/ScriptCommands.h"
#include "objective_specific.h"
#include <CFireManager.h>
#include <CStreaming.h>
#include <CTheScripts.h>
#include <CWorld.h>
#include <corecrt_math_defines.h>
#include <entity.h>
#include <lua_define_type.h>
#include <matrix_utils.h>
#include <popups/vehicle_selector.h>
#include <project_player.h>
#include <scenes_manager.h>
#include <string_utils.h>
#include <utils/imgui_configurate.h>

void components::Firework::sol_lua_register(sol::state_view lua_state) {
	auto ut = lua_state.new_usertype<Firework>("FireworkComponent");
	SOL_LUA_FOR_EACH(SOL_LUA_BIND_MEMBER_ACTION, ut, components::Firework, cast, typeFirework, typeExplosion, sizeFire,
	                 propagationFire);
}

components::Firework::Firework()
	: Component(TYPE) {}

inline nlohmann::json components::Firework::to_json() const {
	auto j = this->Component::to_json();
	j["typeFirework"] = typeFirework;
	j["typeExplosion"] = typeExplosion;
	j["sizeFire"] = sizeFire;
	j["propagationFire"] = propagationFire;
	return j;
}

void components::Firework::from_json(const nlohmann::json& j) {
	this->Component::from_json(j);
	j.at("typeFirework").get_to(typeFirework);
	j.at("typeExplosion").get_to(typeExplosion);
	j.at("sizeFire").get_to(sizeFire);
	j.at("propagationFire").get_to(propagationFire);
}

void components::Firework::editorRender() {
	const auto availableWidth = ImGui::GetContentRegionAvail().x;

	ImGui::Text(_("type").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	if (ImGui::SliderInt("##typeFirework", &typeFirework, 0, 1, tr(fmt::format("types.{}", typeFirework)).c_str())) {
		this->dirty = true;
	}

	if (this->typeFirework == 0) {
		ImGui::Text(tr("sizeFire").c_str());
		ImGui::SameLine(availableWidth * 0.45f);
		ImGui::SetNextItemWidth(-1.f);
		if (ImGui::SliderInt("##sizeFire", &sizeFire, 0, 5)) {
			this->dirty = true;
		}

		ImGui::Text(tr("propagationFire").c_str());
		ImGui::SameLine(availableWidth * 0.45f);
		ImGui::SetNextItemWidth(-1.f);
		ImGui::DragInt("##propagationFire", &propagationFire, 0.1f, 0, 36);
	} else {
		ImGui::Text(tr("typeExplosion").c_str());
		ImGui::SameLine(availableWidth * 0.45f);
		ImGui::SetNextItemWidth(-1.f);
		ImGui::SliderInt("##typeExplosion", &typeExplosion, 0, 12,
		                 tr(fmt::format("explosions_types.{}", typeExplosion)).c_str());
		if (ImGui::Button(tr("preview_explosion").c_str())) {
			runExplosion();
		}
	}
}

void components::Firework::onStart() {
	Component::onStart();
	this->entity->setGetTransformCallbacks(
		[this]() -> std::array<float, 3> {
			if (this->typeFirework == 0) {
				if (this->fire) {
					const int actualScriptThingIndex = CTheScripts::GetActualScriptThingIndex(this->fire.value(), 5);
					const auto fire = &gFireManager.m_aFires[actualScriptThingIndex];
					return {fire->m_vecPosition.x, fire->m_vecPosition.y, fire->m_vecPosition.z};
				}
			} else {
				if (this->m_editorExplosionObject) {
					auto& position = this->m_editorExplosionObject->GetPosition();
					return {position.x, position.y, position.z};
				}
			}
			return {0.0f, 0.0f, 0.0f};
		},
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
			if (this->typeFirework == 0) {
				if (this->fire) {
					const int actualScriptThingIndex = CTheScripts::GetActualScriptThingIndex(this->fire.value(), 5);
					auto& fire = gFireManager.m_aFires[actualScriptThingIndex];
					fire.m_vecPosition = {position[0], position[1], position[2]};
					CMatrix newMatrix(&fire.m_pFxSystem->m_localMatrix, true);
					newMatrix.SetTranslateOnly(position[0], position[1], position[2]);
					newMatrix.UpdateRW(&fire.m_pFxSystem->m_localMatrix);
				}
			} else if (this->m_editorExplosionObject) {
				this->m_editorExplosionObject->SetPosn(position[0], position[1], position[2]);
			}
		},
		[this](const CQuaternion rotation) {
			// if (this->handle) {
		    // 	this->handle->m_matrix->SetRotate(rotation);
		    // }
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

void components::Firework::onUpdate(float deltaTime) {
	Component::onUpdate(deltaTime);
	if (this->dirty) {
		spawn();
	}
	this->dirty = false;
}

void components::Firework::onReset() {
	Component::onReset();
	despawn();
	this->onSpawnedConnection.reset();
	this->onDespawnedConnection.reset();
}

void components::Firework::updateRotation() {
	// if (this->handle) {
	// 	auto rotation = this->entity->rotation;
	// 	this->handle->m_matrix->SetRotate(this->entity->rotation);
	// 	this->handle->UpdateRwMatrix();
	// 	this->handle->UpdateRwFrame();
	// }
}

void components::Firework::updatePosition() {
	// if (this->handle) {
	// 	auto& position = this->entity->position;
	// 	this->handle->SetPosn(position[0], position[1], position[2]);
	// }
}

void components::Firework::runExplosion() {
	plugin::Command<plugin::Commands::ADD_EXPLOSION>(this->entity->position[0], this->entity->position[1],
	                                                 this->entity->position[2], this->typeExplosion);
}

void components::Firework::spawn() {
	despawn();

	if (this->typeFirework == 0) {
		int scriptFire;
		plugin::Command<plugin::Commands::START_SCRIPT_FIRE>(
			this->entity->position[0], this->entity->position[1], this->entity->position[2],
			IS_PLAYING ? 0 : this->propagationFire, this->sizeFire, &scriptFire);
		if (scriptFire != -1) {
			this->fire = scriptFire;
		}
		if (!IS_PLAYING) {
			const int actualScriptThingIndex = CTheScripts::GetActualScriptThingIndex(this->fire.value(), 5);
			const auto fire = &gFireManager.m_aFires[actualScriptThingIndex];
			fire->m_nNumGenerationsAllowed = 0;
		}
	} else {
		if (IS_PLAYING) {
			runExplosion();
		} else {
			CStreaming::RequestModel(1654, 0);
			CStreaming::LoadAllRequestedModels(false);

			this->m_editorExplosionObject = std::shared_ptr<CObject>(CObject::Create(1654), [](CObject* obj) {
				if (obj) {
					plugin::Command<plugin::Commands::DELETE_OBJECT>(obj);
				}
			});
			this->m_editorExplosionObject->SetPosn(this->entity->position[0], this->entity->position[1],
			                                       this->entity->position[2]);
			this->m_editorExplosionObject->m_bUsesCollision = false;
			CWorld::Add(&*this->m_editorExplosionObject);

			CStreaming::SetMissionDoesntRequireModel(1654);
		}
	}

	onSpawned();
}

void components::Firework::despawn() {

	if (this->fire) {
		const int actualScriptThingIndex = CTheScripts::GetActualScriptThingIndex(this->fire.value(), 5);
		if (actualScriptThingIndex != -1) {
			gFireManager.RemoveScriptFire(static_cast<short>(actualScriptThingIndex));
		}
		this->fire.reset();
	}
	this->m_editorExplosionObject.reset();
	onDespawned();
}