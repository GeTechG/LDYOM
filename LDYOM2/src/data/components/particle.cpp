#define IMGUI_DEFINE_MATH_OPERATORS
#include "particle.h"
#include "extensions/ScriptCommands.h"
#include "objective_specific.h"
#include <CStreaming.h>
#include <CTheScripts.h>
#include <FxManager_c.h>
#include <corecrt_math_defines.h>
#include <entity.h>
#include <lua_define_type.h>
#include <matrix_utils.h>
#include <popups/vehicle_selector.h>
#include <project_player.h>
#include <scenes_manager.h>
#include <string_utils.h>
#include <utils/imgui_configurate.h>

void components::Particle::sol_lua_register(sol::state_view lua_state) {
	sol_lua_register_enum_DirtyFlags(lua_state);
	auto ut = lua_state.new_usertype<Particle>("ParticleComponent");
	SOL_LUA_FOR_EACH(SOL_LUA_BIND_MEMBER_ACTION, ut, components::Particle, cast, particleType);
}

components::Particle::Particle()
	: Component(TYPE) {}

inline nlohmann::json components::Particle::to_json() const {
	auto j = this->Component::to_json();
	j["particleType"] = particleType;
	return j;
}

void components::Particle::from_json(const nlohmann::json& j) {
	this->Component::from_json(j);
	j.at("particleType").get_to(particleType);
}

void components::Particle::editorRender() {
	const auto availableWidth = ImGui::GetContentRegionAvail().x;

	ImGui::Text(_("type").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	if (ImGui::BeginCombo("##particleType", particleType.c_str())) {
		for (const auto& particleName : ModelsManager::getParticlesNames()) {
			if (ImGui::Selectable(particleName.c_str(), particleName == particleType)) {
				particleType = particleName;
				dirty |= Type;
			}
		}
		ImGui::EndCombo();
	}
}

void components::Particle::onStart() {
	Component::onStart();
	this->entity->setGetTransformCallbacks(
		[this]() -> std::array<float, 3> {
			if (this->handle) {
				auto actual = CTheScripts::GetActualScriptThingIndex(this->handle.value(), 1);
				const auto fxSystem =
					static_cast<FxSystem_c*>(CTheScripts::ScriptEffectSystemArray[actual].m_pFxSystem);

				if (fxSystem) {
					RwMatrix* matrix = &fxSystem->m_localMatrix;
					if (matrix) {
						auto& position = matrix->pos;
						return {position.x, position.y, position.z};
					}
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
			if (this->handle) {
				auto actual = CTheScripts::GetActualScriptThingIndex(this->handle.value(), 1);
				const auto fxSystem =
					static_cast<FxSystem_c*>(CTheScripts::ScriptEffectSystemArray[actual].m_pFxSystem);

				if (fxSystem) {
					CMatrix matrix(&fxSystem->m_localMatrix, true);
					matrix.SetTranslateOnly(position[0], position[1], position[2]);
					matrix.UpdateRW(&fxSystem->m_localMatrix);
				}
			}
		},
		[this](const CQuaternion rotation) {
			if (this->handle) {
				auto actual = CTheScripts::GetActualScriptThingIndex(this->handle.value(), 1);
				const auto fxSystem =
					static_cast<FxSystem_c*>(CTheScripts::ScriptEffectSystemArray[actual].m_pFxSystem);

				if (fxSystem) {
					CMatrix matrix(&fxSystem->m_localMatrix, true);
					matrix.SetRotate(rotation);
					matrix.UpdateRW(&fxSystem->m_localMatrix);
				}
			}
		},
		[this](const std::array<float, 3>& scale) {
			if (this->handle) {
				auto actual = CTheScripts::GetActualScriptThingIndex(this->handle.value(), 1);
				const auto fxSystem =
					static_cast<FxSystem_c*>(CTheScripts::ScriptEffectSystemArray[actual].m_pFxSystem);

				if (fxSystem) {
					CMatrix matrix(&fxSystem->m_localMatrix, true);
					scaleMatrix(matrix, scale);
					matrix.UpdateRW(&fxSystem->m_localMatrix);
				}
			}
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

void components::Particle::onUpdate(float deltaTime) {
	Component::onUpdate(deltaTime);
	if (this->dirty & Rotation) {
		updateRotation();
	}
	if (this->dirty & Position) {
		updatePosition();
	}
	if (this->dirty & Type) {
		spawn();
	}
	dirty = None;
}

void components::Particle::onReset() {
	Component::onReset();
	despawn();
	this->onSpawnedConnection.reset();
	this->onDespawnedConnection.reset();
}

void components::Particle::updateRotation() {
	if (this->handle) {
		auto actual = CTheScripts::GetActualScriptThingIndex(this->handle.value(), 1);
		const auto fxSystem = static_cast<FxSystem_c*>(CTheScripts::ScriptEffectSystemArray[actual].m_pFxSystem);

		if (fxSystem) {
			CMatrix matrix;
			matrix.SetRotate(this->entity->rotation);
			matrix.SetTranslateOnly(this->entity->position[0], this->entity->position[1], this->entity->position[2]);
			matrix.UpdateRW(&fxSystem->m_localMatrix);
		}
	}
}

void components::Particle::updatePosition() {
	if (this->handle) {
		auto actual = CTheScripts::GetActualScriptThingIndex(this->handle.value(), 1);
		const auto fxSystem = static_cast<FxSystem_c*>(CTheScripts::ScriptEffectSystemArray[actual].m_pFxSystem);

		if (fxSystem) {
			CMatrix matrix;
			matrix.SetRotate(this->entity->rotation);
			matrix.SetTranslateOnly(this->entity->position[0], this->entity->position[1], this->entity->position[2]);
			matrix.UpdateRW(&fxSystem->m_localMatrix);
		}
	}
}

void components::Particle::spawn() {
	if (!ModelsManager::validateParticleName(this->particleType))
		return;

	despawn();

	RwMatrix mat;
	const auto fxSystem = g_fxMan.CreateFxSystem(const_cast<char*>(this->particleType.c_str()), &mat, nullptr, 1);
	const auto scriptEffectSystem = CTheScripts::AddScriptEffectSystem(fxSystem);
	fxSystem->Play();
	this->handle = scriptEffectSystem;

	updatePosition();
	updateRotation();

	onSpawned();
}

void components::Particle::despawn() {
	if (handle) {
		auto actual = CTheScripts::GetActualScriptThingIndex(this->handle.value(), 1);
		const auto fxSystem = static_cast<FxSystem_c*>(CTheScripts::ScriptEffectSystemArray[actual].m_pFxSystem);

		if (fxSystem) {
			g_fxMan.DestroyFxSystem(fxSystem);
			try {
				CTheScripts::RemoveScriptEffectSystem(handle.value());
			} catch (std::exception& e) {
			}
		}
		this->handle = std::nullopt;
	}
	onDespawned();
}