#pragma once
#include "objectives_container.h"
#include "scene_info.h"
#include <scenes_manager.h>

struct Scene {
	SceneInfo info;
	ObjectivesContainer objectives;
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Scene, info, objectives)
};
