#include "entity.h"
#include <components_manager.h>

void Entity::addComponent(std::shared_ptr<Component> component) {
	component->entity = this;
	components.push_back(std::move(component));
}

std::shared_ptr<Component> Entity::getComponent(const std::string_view type) {
	for (const auto& component : components) {
		if (component->getType() == type) {
			return component;
		}
	}
	return nullptr;
}

void Entity::onUpdate(float deltaTime) {
	for (auto& component : components) {
		if (!component->isInitialized) {
			component->onStart();
		}
		component->onUpdate(deltaTime);
	}
}

void Entity::reset() {
	for (auto& component : components) {
		component->onReset();
	}
	for (auto& component : components) {
		component->onStart();
	}
}

void Entity::sol_lua_register(sol::state_view lua_state) {
	auto ut = lua_state.new_usertype<Entity>("Entity");
	SOL_LUA_FOR_EACH(SOL_LUA_BIND_MEMBER_ACTION, ut, Entity, name, position, rotation, scale, getComponents)
}

void to_json(nlohmann::json& j, const Entity& p) {
	auto componentsArray = nlohmann::json::array();
	for (const auto& component : p.components) {
		componentsArray.push_back(component->to_json());
	}
	j = {{"name", p.name},
	     {"position", p.position},
	     {"rotation", p.rotation},
	     {"scale", p.scale},
	     {"components", componentsArray}};
}

void from_json(const nlohmann::json& j, Entity& p) {
	j.at("name").get_to(p.name);
	j.at("position").get_to(p.position);
	j.at("rotation").get_to(p.rotation);
	j.at("scale").get_to(p.scale);
	if (j.contains("components") && j["components"].is_array()) {
		p.components.clear();
		for (const auto& componentJson : j["components"]) {
			if (componentJson.is_object()) {
				std::shared_ptr<Component> component =
					ComponentsManager::instance().createComponent(componentJson["type"].get<std::string>());
				component->from_json(componentJson);
				component->entity = &p;
				p.components.emplace_back(std::move(component));
			} else {
				throw std::invalid_argument("Invalid JSON format for Entity components");
			}
		}
	}
}