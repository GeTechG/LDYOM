#pragma once
#include "objective.h"
#include <objectives_manager.h>

struct ObjectivesContainer {
	std::vector<Objective> data;
};

namespace nlohmann {
void to_json(json& j, const ObjectivesContainer& p);
void from_json(const json& j, ObjectivesContainer& p);
} // namespace nlohmann
