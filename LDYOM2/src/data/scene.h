#pragma once
#include "entity.h"
#include "objectives_container.h"
#include "scene_info.h"

struct Scene {
	SceneInfo info;
	ObjectivesContainer objectives;
	std::vector<std::unique_ptr<Entity>> entities;
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Scene, info, objectives, entities)
};

namespace nlohmann {
template <> struct adl_serializer<std::unique_ptr<Entity>> {
	static void to_json(json& j, const std::unique_ptr<Entity>& entity) {
		if (entity) {
			j = *entity;
		} else {
			j = nullptr;
		}
	}

	static void from_json(const json& j, std::unique_ptr<Entity>& entity) {
		if (j.is_null()) {
			entity.reset();
		} else {
			entity = std::make_unique<Entity>();
			j.get_to(*entity);
		}
	}
};
} // namespace nlohmann