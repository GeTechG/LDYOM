#pragma once
#include "entity.h"
#include "objectives_container.h"
#include "scene_info.h"

struct Scene {
	SceneInfo info;
	ObjectivesContainer objectives;
	std::vector<Entity> entities;
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Scene, info, objectives, entities)
};
