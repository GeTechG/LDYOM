#pragma once
#include "objective.h"
#include "scene_info.h"


struct Scene {
	SceneInfo info;
	std::vector<Objective> objectives;
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Scene, info)
};
