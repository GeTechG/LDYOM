#include "objectives_container.h"
#include <objectives_manager.h>

namespace nlohmann {
void to_json(json& j, const ObjectivesContainer& p) {
	j = json::array();
	for (const auto& objective : p.data) {
		j.push_back(objective);
	}
}

void from_json(const json& j, ObjectivesContainer& p) {
	p.data.clear();
	for (const auto& item : j) {
		if (item.is_object()) {
			Objective objective = ObjectivesManager::instance().createObjective(item["type"].get<std::string>());
			item.get_to(objective);
			p.data.push_back(objective);
		} else {
			throw std::invalid_argument("Invalid JSON format for ObjectivesContainer");
		}
	}
}
} // namespace nlohmann