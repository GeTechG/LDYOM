#define IMGUI_DEFINE_MATH_OPERATORS
#include "object.h"
#include "extensions/ScriptCommands.h"
#include "objective_specific.h"
#include <CStreaming.h>
#include <corecrt_math_defines.h>
#include <entity.h>
#include <lua_define_type.h>
#include <popups/vehicle_selector.h>
#include <project_player.h>
#include <scenes_manager.h>
#include <string_utils.h>
#include <utils/imgui_configurate.h>

void components::Object::sol_lua_register(sol::state_view lua_state) {
	sol_lua_register_enum_DirtyFlags(lua_state);
	auto ut = lua_state.new_usertype<Object>("ObjectComponent");
	SOL_LUA_FOR_EACH(SOL_LUA_BIND_MEMBER_ACTION, ut, components::Object, cast, model, getObjectRef);
}

components::Object::Object()
	: Component(TYPE) {}

inline nlohmann::json components::Object::to_json() const {
	auto j = this->Component::to_json();
	j["model"] = model;
	return j;
}

void components::Object::from_json(const nlohmann::json& j) {
	this->Component::from_json(j);
	j.at("model").get_to(model);
}

void components::Object::editorRender() {
	const auto availableWidth = ImGui::GetContentRegionAvail().x;

	ImGui::Text(_("model").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	if (ImGui::InputInt("##model", &model, 0, 0)) {
		this->dirty |= Model;
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
		[this]() -> std::array<float, 3> {
			// if (this->vehicle) {
		    // 	return (std::array<float,3>)&this->vehicle->GetMatrix()->rot;
		    // }
			return {0.0f, 0.0f, 0.0f};
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
				this->handle->SetPosn(position[0], position[1], position[2]);
			}
		},
		[this](const std::array<float, 3>& rotation) {
			if (this->handle) {
				plugin::Command<plugin::Commands::SET_OBJECT_ROTATION>(this->getObjectRef(), rotation[0], rotation[1],
			                                                           rotation[2]);
			}
		},
		[this](const std::array<float, 3>& scale) {});
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
		auto rotation = this->entity->rotation;
		plugin::Command<plugin::Commands::SET_OBJECT_ROTATION>(this->getObjectRef(), rotation[0], rotation[1],
		                                                       rotation[2]);
	}
}

void components::Object::updatePosition() {
	if (this->handle) {
		auto& position = this->entity->position;
		this->handle->SetPosn(position[0], position[1], position[2]);
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
	updatePosition();
	updateRotation();

	CStreaming::SetMissionDoesntRequireModel(this->model);

	if (!IS_PLAYING) {
		plugin::Command<plugin::Commands::FREEZE_OBJECT_POSITION>(newObject, 1);
		plugin::Command<plugin::Commands::SET_OBJECT_DYNAMIC>(newObject, 0);
		plugin::Command<plugin::Commands::SET_OBJECT_PROOFS>(newObject, 1, 1, 1, 1, 1);
	}

	onSpawned();
}

void components::Object::despawn() {
	if (handle) {
		handle = nullptr;
	}
	onDespawned();
}