#include "health_bar.h"
#include "actor.h"
#include <CText.h>
#include <counter_service.h>
#include <imgui_stdlib.h>
#include <project_player.h>

void components::HealthBar::sol_lua_register(sol::state_view lua_state) {
	auto ut = lua_state.new_usertype<HealthBar>("HealthBarComponent");
	SOL_LUA_FOR_EACH(SOL_LUA_BIND_MEMBER_ACTION, ut, components::HealthBar, cast, defaultTitle, title);
}

components::HealthBar::HealthBar()
	: Component(TYPE) {}

nlohmann::json components::HealthBar::to_json() const {
	auto j = this->Component::to_json();
	j["defaultTitle"] = defaultTitle;
	j["title"] = title;
	return j;
}

void components::HealthBar::from_json(const nlohmann::json& j) {
	this->Component::from_json(j);
	j.at("defaultTitle").get_to(defaultTitle);
	j.at("title").get_to(title);
}

void components::HealthBar::editorRender() {
	const auto availableWidth = ImGui::GetContentRegionAvail().x;

	ImGui::Text(tr("default_title").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::Checkbox("##defaultTitle", &defaultTitle);

	if (!defaultTitle) {
		ImGui::Text(tr("title").c_str());
		ImGui::SameLine(availableWidth * 0.45f);
		ImGui::SetNextItemWidth(-1.f);
		ImGui::InputText("##title", &title);
	}
}

void components::HealthBar::onStart() {
	Component::onStart();
	if (IS_PLAYING) {
		if (this->entity->hasComponent(components::Actor::TYPE)) {
			auto actor = components::Actor::cast(this->entity->getComponent(components::Actor::TYPE));
			if (actor) {
				actor->onSpawned.connect(
					[this, actor]() {
						auto title = this->defaultTitle ? std::string(TheText.Get("HEALTH")) : this->title;
						if (actor->ped) {
							auto initialValue = int(actor->ped->m_fHealth / actor->ped->m_fMaxHealth * 100.0f);
							auto counterId = CounterService::instance().addBarCounter(title, initialValue);
							if (counterId >= 0) {
								this->counterId = counterId;
							}
						}
						rocket::current_connection().disconnect();
					},
					rocket::connection_flags::queued_connection);
			}
		}
	}
}

void components::HealthBar::onUpdate(float deltaTime) {
	Component::onUpdate(deltaTime);
	if (IS_PLAYING) {
		if (auto actor = components::Actor::cast(this->entity->getComponent(components::Actor::TYPE))) {
			if (actor->ped && this->counterId) {
				auto health = int(actor->ped->m_fHealth / actor->ped->m_fMaxHealth * 100.0f);
				CounterService::instance().updateCounter(*this->counterId, health);
			}
		}
	}
}

void components::HealthBar::onReset() {
	Component::onReset();
	if (this->counterId) {
		CounterService::instance().clearCounter(*this->counterId);
		this->counterId.reset();
	}
}

Dependencies components::HealthBar::getDependencies() { return Dependencies{{components::Actor::TYPE}, true}; }
