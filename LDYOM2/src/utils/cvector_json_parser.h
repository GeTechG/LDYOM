#pragma once
#include "CVector.h"
#include "nlohmann/json.hpp"

namespace nlohmann {
template <> struct adl_serializer<CVector> {
	static void to_json(json& j, const CVector& v) { j = {v.x, v.y, v.z}; }

	static void from_json(const json& j, CVector& v) {
		if (j.is_array() && j.size() == 3) {
			v.x = j[0].get<float>();
			v.y = j[1].get<float>();
			v.z = j[2].get<float>();
		} else {
			throw std::invalid_argument("CVector must be an array of 3 floats");
		}
	}
};
} // namespace nlohmann