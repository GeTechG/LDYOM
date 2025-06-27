#include "entity.h"
#include <components_manager.h>
#include <cquat_json.h>

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

void Entity::setGetTransformCallbacks(std::function<std::array<float, 3>()> positionCallback,
                                      std::function<CQuaternion()> rotationCallback,
                                      std::function<std::array<float, 3>()> scaleCallback) {
	getPositionCallback = positionCallback;
	getRotationCallback = rotationCallback;
	getScaleCallback = scaleCallback;
}
void Entity::setSetTransformCallbacks(std::function<void(std::array<float, 3>)> positionCallback,
                                      std::function<void(CQuaternion)> rotationCallback,
                                      std::function<void(std::array<float, 3>)> scaleCallback) {
	setPositionCallback = positionCallback;
	setRotationCallback = rotationCallback;
	setScaleCallback = scaleCallback;
}

void Entity::updateSetTransformCallbacks() {
	if (setPositionCallback) {
		setPositionCallback(this->position);
	}
	if (setRotationCallback) {
		setRotationCallback(this->rotation);
	}
	if (setScaleCallback) {
		setScaleCallback(this->scale);
	}
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

void Entity::removeComponent(size_t index) {
	if (index < components.size()) {
		components.erase(components.begin() + index);
	}
}

void Entity::duplicateComponent(size_t index) {
	if (index < components.size()) {
		auto& originalComponent = components[index];
		auto json = originalComponent->to_json();
		auto newComponent = ComponentsManager::instance().createComponent(originalComponent->getType());
		newComponent->from_json(json);
		newComponent->entity = this;
		components.insert(components.begin() + index + 1, newComponent);
	}
}

void Entity::moveComponentUp(size_t index) {
	if (index > 0 && index < components.size()) {
		std::swap(components[index], components[index - 1]);
	}
}

void Entity::moveComponentDown(size_t index) {
	if (index < components.size() - 1) {
		std::swap(components[index], components[index + 1]);
	}
}

void Entity::sol_lua_register(sol::state_view lua_state) {
	auto ut = lua_state.new_usertype<Entity>("Entity");
	SOL_LUA_FOR_EACH(SOL_LUA_BIND_MEMBER_ACTION, ut, Entity, name, id, position, rotation, scale, areaId, getComponents)
}

void to_json(nlohmann::json& j, const Entity& p) {
	auto componentsArray = nlohmann::json::array();
	for (const auto& component : p.components) {
		componentsArray.push_back(component->to_json());
	}
	j = {{"name", p.name},   {"id", p.id},         {"position", p.position},       {"rotation", p.rotation},
	     {"scale", p.scale}, {"areaId", p.areaId}, {"components", componentsArray}};
}

void from_json(const nlohmann::json& j, Entity& p) {
	j.at("name").get_to(p.name);
	j.at("id").get_to(p.id);
	j.at("position").get_to(p.position);
	j.at("rotation").get_to(p.rotation);
	j.at("scale").get_to(p.scale);
	j.at("areaId").get_to(p.areaId);
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