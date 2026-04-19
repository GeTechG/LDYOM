#define IMGUI_DEFINE_MATH_OPERATORS
#include "object.h"
#include "extensions/ScriptCommands.h"
#include "objective_specific.h"
#include <CStreaming.h>
#include <CTheScripts.h>
#include <corecrt_math_defines.h>
#include <entity.h>
#include <lua_define_type.h>
#include <popups/object_selector.h>
#include <project_player.h>
#include <scenes_manager.h>
#include <string_utils.h>
#include <utils/imgui_configurate.h>

void components::Object::sol_lua_register(sol::state_view lua_state) {
	sol_lua_register_enum_DirtyFlags(lua_state);
	auto ut = lua_state.new_usertype<Object>("ObjectComponent");
	SOL_LUA_FOR_EACH(SOL_LUA_BIND_MEMBER_ACTION, ut, components::Object, cast, model, scale, getObjectRef);
}

components::Object::Object()
	: Component(TYPE) {}

inline nlohmann::json components::Object::to_json() const {
	auto j = this->Component::to_json();
	j["model"] = model;
	j["scale"] = scale;
	return j;
}

void components::Object::from_json(const nlohmann::json& j) {
	this->Component::from_json(j);
	j.at("model").get_to(model);
	if (j.contains("scale")) {
		j.at("scale").get_to(scale);
	}
}

void components::Object::editorRender() {
	const auto availableWidth = ImGui::GetContentRegionAvail().x;
	const float btnW = ImGui::GetFontSize() * 2.f;

	ImGui::Text(_("model").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-btnW - ImGui::GetStyle().ItemSpacing.x);
	if (ImGui::InputInt("##model", &model, 0, 0)) {
		this->dirty |= Model;
	}
	ImGui::SameLine();
	if (ImGui::Button(ICON_FA_CUBE "##objSelect", ImVec2(btnW, 0.f))) {
		PopupObjectSelector::showPopup();
	}
	PopupObjectSelector::renderPopup([this](int selectedModel) {
		this->model = selectedModel;
		this->dirty |= Model;
	});

	ImGui::Text(_("scale").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	if (ImGui::DragFloat("##scale", &scale, 0.01f, 0.0f, 0.0f, "%.3f")) {
		this->dirty |= Scale;
	}
}

void components::Object::onStart() {
	Component::onStart();
	this->entity->setGetTransformCallbacks(
		[this]() -> std::array<float, 3> {
			if (this->handle) {
				auto& position = this->handle->GetPosition();
				return {position.x, position.y, position.z};
			}
			return {0.0f, 0.0f, 0.0f};
		},
		[this]() -> CQuaternion {
			// if (this->vehicle) {
		    // 	return (std::array<float,3>)&this->vehicle->GetMatrix()->rot;
		    // }
			return {};
		});
	this->entity->setSetTransformCallbacks(
		[this](const std::array<float, 3>& position) {
			if (this->handle) {
				// Proper matrix update following CObject::Teleport implementation
				this->handle->Remove();

				// Update position in matrix or simple transform
				CMatrixLink* matrix = this->handle->GetMatrix();
				if (matrix) {
					matrix->GetPosition().x = position[0];
					matrix->GetPosition().y = position[1];
					matrix->GetPosition().z = position[2];
				} else {
					this->handle->m_placement.m_vPosn.x = position[0];
					this->handle->m_placement.m_vPosn.y = position[1];
					this->handle->m_placement.m_vPosn.z = position[2];
				}

				// Update RenderWare
				if (this->handle->m_pRwObject) {
					if (matrix) {
						matrix->UpdateRW();
					}
					this->handle->UpdateRwMatrix();
				}
				this->handle->UpdateRwFrame();
				this->handle->Add();
			}
		},
		[this](const CQuaternion rotation) {
			if (this->handle) {
				// Proper rotation update with Remove/Add
				this->handle->Remove();

				CMatrixLink* matrix = this->handle->GetMatrix();
				if (matrix) {
					matrix->SetRotate(rotation);
					matrix->UpdateRW();
				}

				if (this->handle->m_pRwObject) {
					this->handle->UpdateRwMatrix();
				}
				this->handle->UpdateRwFrame();
				this->handle->Add();
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

void components::Object::onUpdate(float deltaTime) {
	Component::onUpdate(deltaTime);
	if (this->dirty & Rotation) {
		updateRotation();
	}
	if (this->dirty & Position) {
		updatePosition();
	}
	if (this->dirty & Model) {
		spawn();
	}
	if (this->dirty & Scale) {
		if (this->handle) {
			this->handle->m_fScale = this->scale;
		}
	}
	dirty = None;
}

void components::Object::onReset() {
	Component::onReset();
	despawn();
	this->onSpawnedConnection.reset();
	this->onDespawnedConnection.reset();
}

void components::Object::updateRotation() {
	if (this->handle) {
		// Proper rotation update following CObject::Teleport pattern
		this->handle->Remove();

		CMatrixLink* matrix = this->handle->GetMatrix();
		if (matrix) {
			matrix->SetRotate(this->entity->rotation);
			matrix->UpdateRW();
		}

		if (this->handle->m_pRwObject) {
			this->handle->UpdateRwMatrix();
		}
		this->handle->UpdateRwFrame();
		this->handle->Add();
	}
}

void components::Object::updatePosition() {
	if (this->handle) {
		// Proper position update following CObject::Teleport pattern
		this->handle->Remove();

		auto& position = this->entity->position;

		// Update position in matrix or simple transform
		CMatrixLink* matrix = this->handle->GetMatrix();
		if (matrix) {
			matrix->GetPosition().x = position[0];
			matrix->GetPosition().y = position[1];
			matrix->GetPosition().z = position[2];
		} else {
			this->handle->m_placement.m_vPosn.x = position[0];
			this->handle->m_placement.m_vPosn.y = position[1];
			this->handle->m_placement.m_vPosn.z = position[2];
		}

		// Update RenderWare
		if (this->handle->m_pRwObject) {
			if (matrix) {
				matrix->UpdateRW();
			}
			this->handle->UpdateRwMatrix();
		}
		this->handle->UpdateRwFrame();
		this->handle->Add();
	}
}

void components::Object::spawn() {
	if (!ModelsManager::validObjectModel(this->model))
		return;

	CStreaming::RequestModel(this->model, GAME_REQUIRED);
	CStreaming::LoadAllRequestedModels(false);

	auto position = this->entity->position;

	int newObject;
	plugin::Command<plugin::Commands::CREATE_OBJECT_NO_OFFSET>(this->model, position[0], position[1], position[2],
	                                                           &newObject);
	plugin::Command<plugin::Commands::SET_OBJECT_AREA_VISIBLE>(newObject, this->entity->areaId);

	CObject* object = CPools::GetObject(newObject);
	this->handle = std::shared_ptr<CObject>(object, [](CObject* object) {
		if (!ScenesManager::instance().isRestartGame()) {
			auto ref = CPools::GetObjectRef(object);
			plugin::Command<plugin::Commands::DELETE_OBJECT>(ref);
		}
	});
	this->handle->m_fScale = this->scale;
	this->entity->updateSetTransformCallbacks();

	CStreaming::SetMissionDoesntRequireModel(this->model);

	if (!IS_PLAYING) {
		plugin::Command<plugin::Commands::FREEZE_OBJECT_POSITION>(newObject, 1);
		plugin::Command<plugin::Commands::SET_OBJECT_DYNAMIC>(newObject, 0);
		plugin::Command<plugin::Commands::SET_OBJECT_PROOFS>(newObject, 1, 1, 1, 1, 1);
	} else {
		// Mission objects keep ticking physics even outside the streamer radius, so one
		// spawned far from the player falls through ungenerated world collision. Freeze
		// position + disable collision-load wait until the player gets close enough.
		constexpr float kUnfreezeRadius = 100.0f;
		const CVector spawnPos(position[0], position[1], position[2]);
		if (DistanceBetweenPoints(spawnPos, FindPlayerPed()->GetPosition()) > kUnfreezeRadius) {
			plugin::Command<plugin::Commands::FREEZE_OBJECT_POSITION_AND_DONT_LOAD_COLLISION>(newObject, 1);
			ProjectPlayer::instance().projectTasklist->add_task(
				[](const Object* _this, float radius) -> ktwait {
					while (_this->handle && IS_PLAYING) {
						if (DistanceBetweenPoints(_this->handle->GetPosition(), FindPlayerPed()->GetPosition()) <
						    radius) {
							plugin::Command<plugin::Commands::FREEZE_OBJECT_POSITION_AND_DONT_LOAD_COLLISION>(
								_this->getObjectRef(), 0);
							break;
						}
						co_await 1;
					}
				},
				this, kUnfreezeRadius);
		}
	}

	onSpawned();
}

void components::Object::despawn() {
	if (handle) {
		handle->m_fScale = 1.0f;
		handle = nullptr;
	}
	onDespawned();
}