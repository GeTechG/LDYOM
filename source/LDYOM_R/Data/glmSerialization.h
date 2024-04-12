#pragma once
#include <nlohmann/json.hpp>

#include "glm/vec3.hpp"

NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<glm::vec3> {
		static void to_json(json &j, const glm::vec3 &obj) {
			j["x"] = obj.x;
			j["y"] = obj.y;
			j["z"] = obj.z;
		}

		static void from_json(const json &j, glm::vec3 &obj) {
			j.at("x").get_to(obj.x);
			j.at("y").get_to(obj.y);
			j.at("z").get_to(obj.z);
		}
	};

NLOHMANN_JSON_NAMESPACE_END
