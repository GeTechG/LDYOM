#include "actor_animation.h"
#include "actor.h"
#include "entity.h"
#include "extensions/ScriptCommands.h"
#include <models_manager.h>
#include <project_player.h>
#include <task_manager.h>

void components::ActorAnimation::sol_lua_register(sol::state_view lua_state) {
	auto ut = lua_state.new_usertype<ActorAnimation>("ActorAnimationComponent");
	SOL_LUA_FOR_EACH(SOL_LUA_BIND_MEMBER_ACTION, ut, components::ActorAnimation, cast, packName, animName, isLooped,
	                 playAnimation);
}

components::ActorAnimation::ActorAnimation()
	: Component(TYPE) {}

void components::ActorAnimation::playAnimation() {
	auto callback = [](const ActorAnimation* _this) -> ktwait {
		const auto actor = Actor::cast(_this->entity->getComponent(Actor::TYPE));
		if (ModelsManager::validatePedAnimation(_this->packName, _this->animName)) {
			const auto isCustom = _this->packName != "PED";
			if (isCustom) {
				plugin::Command<plugin::Commands::REQUEST_ANIMATION>(_this->packName.c_str());
				while (!plugin::Command<plugin::Commands::HAS_ANIMATION_LOADED>(_this->packName.c_str())) {
					co_await 1;
				}
			}
			if (actor && actor->ped) {
				plugin::Command<plugin::Commands::TASK_PLAY_ANIM>(actor->getPedRef(), _this->animName.c_str(),
				                                                  _this->packName.c_str(), 4.0f, _this->isLooped,
				                                                  !_this->isLooped, !_this->isLooped, false, -1);
			}
			if (isCustom) {
				plugin::Command<plugin::Commands::REMOVE_ANIMATION>(_this->packName.c_str());
			}
		}
		co_await 0;
	};
	if (IS_PLAYING) {
		ProjectPlayer::instance().projectTasklist->add_task(callback, this);
	} else {
		TaskManager::instance().addTask("ActorAnimation::playAnimation", callback, this);
	}
}

void components::ActorAnimation::stopAnimation() {
	const auto actor = Actor::cast(this->entity->getComponent(Actor::TYPE));
	if (actor && actor->ped) {
		plugin::Command<plugin::Commands::CLEAR_CHAR_TASKS>(actor->getPedRef());
	}
}

bool components::ActorAnimation::isPlayingAnimation() {
	const auto actor = Actor::cast(this->entity->getComponent(Actor::TYPE));
	if (actor && actor->ped) {
		return plugin::Command<plugin::Commands::IS_CHAR_PLAYING_ANIM>(actor->getPedRef(), animName.c_str());
	}
	return false;
}

nlohmann::json components::ActorAnimation::to_json() const {
	auto j = this->Component::to_json();
	j["packName"] = packName;
	j["animName"] = animName;
	j["isLooped"] = isLooped;
	return j;
}

void components::ActorAnimation::from_json(const nlohmann::json& j) {
	this->Component::from_json(j);
	j.at("packName").get_to(packName);
	j.at("animName").get_to(animName);
	j.at("isLooped").get_to(isLooped);
}

void components::ActorAnimation::editorRender() {
	const auto availableWidth = ImGui::GetContentRegionAvail().x;

	ImGui::Text(tr("pack_name").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	if (ImGui::BeginCombo("##packName", packName.c_str())) {
		for (const auto& key : ModelsManager::getPedAnimations() | std::views::keys) {
			if (ImGui::Selectable(key.c_str(), key == packName)) {
				packName = key;
				animName = ModelsManager::getPedAnimations().at(key).front();
			}
		}
		ImGui::EndCombo();
	}

	ImGui::Text(tr("anim_name").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	if (ImGui::BeginCombo("##animName", animName.c_str())) {
		if (ModelsManager::getPedAnimations().contains(packName)) {
			for (const auto& anim : ModelsManager::getPedAnimations().at(packName)) {
				if (ImGui::Selectable(anim.c_str(), anim == animName)) {
					animName = anim;
				}
			}
		}
		ImGui::EndCombo();
	}

	ImGui::Text(tr("is_looped").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::Checkbox("##isLooped", &isLooped);

	if (this->isPlayingAnimation()) {
		if (ImGui::Button(fmt::format("{} {}", ICON_FA_STOP, tr("stop_animation")).c_str())) {
			stopAnimation();
		}
	} else {
		if (ImGui::Button(fmt::format("{} {}", ICON_FA_PLAY, tr("play_animation")).c_str())) {
			playAnimation();
		}
	}
}

void components::ActorAnimation::onStart() {
	Component::onStart();
	if (IS_PLAYING) {
		const auto actor = Actor::cast(this->entity->getComponent(Actor::TYPE));
		if (actor) {
			if (actor->ped) {
				playAnimation();
			}
			this->m_pedSpawnedConnection = std::make_optional(actor->onSpawned.connect([this]() { playAnimation(); }));
		}
	}
}

void components::ActorAnimation::onUpdate(float deltaTime) { Component::onUpdate(deltaTime); }

void components::ActorAnimation::onReset() {
	Component::onReset();
	this->m_pedSpawnedConnection.reset();
}

Dependencies components::ActorAnimation::getDependencies() { return Dependencies{{Actor::TYPE}, true}; }