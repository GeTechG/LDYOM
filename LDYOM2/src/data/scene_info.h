#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include <uuid_wrap.h>

struct SceneInfo {
	std::string name;
	std::string id = "";
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(SceneInfo, name, id)
};