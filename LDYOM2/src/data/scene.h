#pragma once
#include "scene_info.h"

struct Scene {
	SceneInfo info;
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Scene, info)
};
