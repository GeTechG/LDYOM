#pragma once
#include <CVector.h>
#include <nlohmann/json.hpp>

NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<CVector> {
		static void to_json(json &j, const CVector &value) {
			j = json{{"x", value.x}, {"y", value.y}, {"z", value.z}};
		}

		static void from_json(const json &j, CVector &value) {
			value.x = j.at("x").get<float>();
			value.y = j.at("y").get<float>();
			value.z = j.at("z").get<float>();
		}
	};

NLOHMANN_JSON_NAMESPACE_END
