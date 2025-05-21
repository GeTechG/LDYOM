#include "entity.h"

namespace nlohmann {
void to_json(json& j, const ComponentsContainer& p) {
	j = json::array();
	for (const auto& component : p.data) {
		j.push_back(component->to_json());
	}
}

void from_json(const json& j, ComponentsContainer& p) {
	if (j.is_array()) {
		p.data.clear();
		for (const auto& componentJson : j) {
			if (componentJson.is_object()) {
				std::shared_ptr<Component> component =
					ComponentsManager::instance().createComponent(componentJson["type"].get<std::string>());
				component->from_json(componentJson);
				p.data.emplace_back(std::move(component));
			} else {
				throw std::invalid_argument("Invalid JSON format for Entity components");
			}
		}
	}
}
} // namespace nlohmann