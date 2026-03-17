#pragma once
#include "global_variable.h"
#include <nlohmann/json.hpp>
#include <string>
#include <uuid_wrap.h>
#include <vector>

struct ProjectInfo {
	std::string name;
	std::string path = "";
	std::string author;
	std::string startSceneId;
	time_t timestamp;
	uuids::uuid uuid = uuids::uuid_system_generator{}();
	std::vector<GlobalVariable> globalVars;
	NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(ProjectInfo, name, author, startSceneId, timestamp, uuid, globalVars)
};
