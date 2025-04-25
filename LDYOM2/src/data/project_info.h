#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include <uuid_wrap.h>

struct ProjectInfo {
	std::string name;
	std::string path = "";
	std::string author;
	time_t timestamp;
	uuids::uuid uuid = uuids::uuid_system_generator{}();
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(ProjectInfo, name, author, timestamp, uuid)
};
