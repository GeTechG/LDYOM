#include "actor.h"
#include "objective_specific.h"
#include "extensions/ScriptCommands.h"
#include <CStreaming.h>
#include <corecrt_math_defines.h>
#include <entity.h>
#include <project_player.h>
#include <string_utils.h>

void components::Actor::sol_lua_register(sol::state_view lua_state) {
	sol_lua_register_enum_DirtyFlags(lua_state);
	auto ut = lua_state.new_usertype<Actor>("ActorComponent");
	SOL_LUA_FOR_EACH(SOL_LUA_BIND_MEMBER_ACTION, ut, components::Actor, cast, isRandomModel, initialDirection, model,
	                 specialModel, isSimpleType, pedType, health, headshotImmune, mustSurvive, ped, spawn, despawn);
}

components::Actor::Actor()
	: Component(TYPE) {
	this->initialDirection = FindPlayerPed()->GetHeading();
}

inline nlohmann::json components::Actor::to_json() const {
	auto j = this->Component::to_json();
	j["isRandomModel"] = isRandomModel;
	j["initialDirection"] = initialDirection;
	j["model"] = model;
	j["specialModel"] = specialModel;
	j["isSimpleType"] = isSimpleType;
	j["pedType"] = pedType;
	j["health"] = health;
	j["headshotImmune"] = headshotImmune;
	j["mustSurvive"] = mustSurvive;
	return j;
}

void components::Actor::from_json(const nlohmann::json& j) {
	this->Component::from_json(j);
	j.at("isRandomModel").get_to(isRandomModel);
	j.at("initialDirection").get_to(initialDirection);
	j.at("model").get_to(model);
	j.at("specialModel").get_to(specialModel);
	j.at("isSimpleType").get_to(isSimpleType);
	j.at("pedType").get_to(pedType);
	j.at("health").get_to(health);
	j.at("headshotImmune").get_to(headshotImmune);
	j.at("mustSurvive").get_to(mustSurvive);
}

void components::Actor::editorRender() {
	const auto availableWidth = ImGui::GetContentRegionAvail().x;
	ImGui::Text("%s", tr("direction").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	if (ImGui::SliderAngle("##direction", &initialDirection, -180.0f, 180.0f, "%.0f°")) {
		dirty |= Direction;
	}

	ImGui::Text("%s", tr("is_random_model").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	if (ImGui::Checkbox("##isRandomModel", &isRandomModel)) {
		dirty |= Model;
	}

	if (!this->isRandomModel) {
		ImGui::Text("%s", tr("is_special").c_str());
		ImGui::SameLine(availableWidth * 0.45f);
		ImGui::SetNextItemWidth(-1.f);
		bool isSpecial = (model == -1);
		if (ImGui::Checkbox("##is_special", &isSpecial)) {
			model = isSpecial ? -1 : 0;
			dirty |= Model;
		}

		const bool isSpecialModel = (this->model == -1);

		ImGui::Text(tr("model").c_str());
		ImGui::SameLine(availableWidth * 0.45f);

		ImGui::BeginGroup(); // Group for combo and button
		const float currentAvailableWidthInGroup = ImGui::GetContentRegionAvail().x;
		const float buttonWidth = ImGui::GetFrameHeight();
		const float spacing = ImGui::GetStyle().ItemSpacing.x;
		float comboWidth = currentAvailableWidthInGroup - buttonWidth - spacing;
		if (comboWidth < 100.0f) {
			comboWidth = 100.0f;
		}
		ImGui::SetNextItemWidth(comboWidth);

		if (isSpecialModel) {
			auto& models = ModelsManager::getPedSpecialModels();
			if (ImGui::BeginCombo("##specialModelCombo", specialModel.c_str())) {
				for (auto& item : models) {
					if (ImGui::Selectable(item.c_str(), item == specialModel)) {
						specialModel = item;
						dirty |= Model;
					}
				}
				ImGui::EndCombo();
			}
		} else {
			auto& pedModels = ModelsManager::getPedModels();
			if (ImGui::BeginCombo("##regularModelCombo", std::to_string(model).c_str())) {
				for (auto& item : pedModels) {
					if (ImGui::Selectable(std::to_string(item).c_str(), item == model)) {
						model = item;
						dirty |= Model;
					}
				}
				ImGui::EndCombo();
			}
		}

		ImGui::SameLine(0.0f, spacing);
		if (ImGui::Button(ICON_FA_SHIRT, ImVec2(buttonWidth, 0))) {
			PopupSkinSelector::showPopup();
		}

		static std::function<void(Skin)> skinSelectorCallback = [this](Skin selected) {
			if (selected.model != -1) {
				this->model = selected.model;
			} else {
				this->specialModel = selected.special;
			}
			dirty |= Model;
		};
		PopupSkinSelector::renderPopup(skinSelectorCallback, isSpecialModel);

		ImGui::EndGroup();
	}

	ImGui::Text(tr("is_simple_type").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	if (ImGui::Checkbox("##isSimpleType", &isSimpleType)) {
		if (isSimpleType) {
			this->pedType = 0; // Neutral
		} else {
			this->pedType = PED_TYPE_CIVMALE;
		}
	}

	ImGui::Text(tr("ped_type").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	if (this->isSimpleType) {
		const auto pedTypeName = tr("simple_types." + std::to_string(this->pedType));
		if (ImGui::BeginCombo("##pedType", pedTypeName.c_str())) {
			for (int i = 0; i < 4; i++) {
				if (ImGui::Selectable(tr("simple_types." + std::to_string(i)).c_str(), this->pedType == i)) {
					this->pedType = i;
				}
			}
			ImGui::EndCombo();
		}
	} else {
		if (ImGui::BeginCombo("##pedType", to_string(static_cast<ePedType>(this->pedType)))) {
			for (int i = 0; i <= PED_TYPE_MISSION8; ++i) {
				if (ImGui::Selectable(to_string(static_cast<ePedType>(i)), this->pedType == i)) {
					this->pedType = i;
				}
			}
			ImGui::EndCombo();
		}
	}

	ImGui::Separator();

	ImGui::Text(tr("health").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::DragFloat("##health", &health, 1.0f, 0.0f, FLT_MAX, "%.0f", ImGuiSliderFlags_ClampOnInput);

	ImGui::Text(tr("headshot_immune").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::Checkbox("##headshotImmune", &headshotImmune);

	ImGui::Text(tr("must_survive").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::Checkbox("##mustSurvive", &mustSurvive);
}

void components::Actor::onStart() {
	Component::onStart();
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

void components::Actor::onUpdate(float deltaTime) {
	Component::onUpdate(deltaTime);
	if (this->dirty & Direction) {
		updateDirection();
	}
	if (this->dirty & Position) {
		updatePosition();
	}
	if (this->dirty & Model) {
		spawn();
	}
	dirty = None;
}

void components::Actor::onReset() {
	Component::onReset();
	despawn();
	this->onSpawnedConnection.reset();
	this->onDespawnedConnection.reset();
}

void components::Actor::updateDirection() {
	if (this->ped) {
		auto heading = this->initialDirection * 180.0f / static_cast<float>(M_PI);
		plugin::Command<plugin::Commands::SET_CHAR_HEADING>(CPools::GetPedRef(this->ped.get()), heading);
	}
}

void components::Actor::updatePosition() {
	if (this->ped) {
		auto& position = this->entity->position;
		this->ped->SetPosn(position[0], position[1], position[2]);
		this->ped->m_nPhysicalFlags.bSubmergedInWater = 0;
		this->ped->m_nPhysicalFlags.bOnSolidSurface = 0;
	}
}

void components::Actor::spawn() {
	int model = this->model;
	std::string specialModel = this->specialModel;
	if (this->isRandomModel) {
		if (rand() % 100 < 50) {
			model = -1; // use special model
			specialModel = ModelsManager::getPedSpecialModels()[rand() % ModelsManager::getPedSpecialModels().size()];
		} else {
			model = ModelsManager::getPedModels()[rand() % ModelsManager::getPedModels().size()];
		}
	}
	if (model == -1 && ModelsManager::validatePedSpecialModel(specialModel)) {
		model = 290;
		CStreaming::RequestSpecialChar(0, specialModel.c_str(), MISSION_REQUIRED);
	} else if (ModelsManager::validatePedModel(model)) {
		CStreaming::RequestModel(model, MISSION_REQUIRED);
	} else {
		return;
	}
	CStreaming::LoadAllRequestedModels(false);

	int pedType = this->pedType;
	if (this->isSimpleType) {
		switch (pedType) {
			case 0: // Neutral
				pedType = PED_TYPE_CIVMALE;
				break;
			case 1: // Friendly
				pedType = PED_TYPE_MISSION2;
				break;
			case 2: // Enemy 1
				pedType = PED_TYPE_MISSION1;
				break;
			case 3: // Enemy 2
				pedType = PED_TYPE_MISSION3;
				break;
		}
	}

	int newPed;
	auto& position = this->entity->position;
	plugin::Command<plugin::Commands::CREATE_CHAR>(pedType, model, position[0], position[1], position[2], &newPed);
	CStreaming::SetMissionDoesntRequireModel(model);

	CPed* ped = CPools::GetPed(newPed);
	this->ped = std::shared_ptr<CPed>(ped, [](CPed* ped) {
		auto ref = CPools::GetPedRef(ped);
		plugin::Command<plugin::Commands::DELETE_CHAR>(ref);
	});
	updatePosition();
	updateDirection();
	ped->m_fMaxHealth = std::max(ped->m_fMaxHealth, this->health);
	ped->m_fHealth = this->health;

	if (this->mustSurvive) {
		ProjectPlayer::instance().projectTasklist->add_task(
			[](const Actor* _this) -> ktwait {
				while (_this->ped && IS_PLAYING) {
					ePedState pedState = _this->ped->m_ePedState;
					if (pedState == PEDSTATE_DEAD || pedState == PEDSTATE_DIE || pedState == PEDSTATE_DIE_BY_STEALTH) {
						ProjectPlayer::instance().failCurrentProject();
						break;
					}
					co_await 1;
				}
			},
			this);
	}
	plugin::Command<plugin::Commands::SET_CHAR_SUFFERS_CRITICAL_HITS>(newPed, !this->headshotImmune);
	plugin::Command<plugin::Commands::SET_CHAR_AREA_VISIBLE>(newPed, this->entity->areaId);

	if (!IS_PLAYING) {
		this->ped->m_bUsesCollision = 0;
		this->ped->m_nPhysicalFlags.bBulletProof = 1;
		this->ped->m_nPhysicalFlags.bCollisionProof = 1;
		this->ped->m_nPhysicalFlags.bExplosionProof = 1;
		this->ped->m_nPhysicalFlags.bFireProof = 1;
		this->ped->m_nPhysicalFlags.bMeleeProof = 1;
		plugin::Command<plugin::Commands::FREEZE_CHAR_POSITION_AND_DONT_LOAD_COLLISION>(newPed, 1);
	}

	onSpawned();
}

void components::Actor::despawn() {
	if (ped) {
		ped = nullptr;
	}
	onDespawned();
}