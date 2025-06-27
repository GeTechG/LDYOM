#pragma once
#include "CQuaternion.h"
#include "nlohmann/json.hpp"
#include <stdexcept>

namespace nlohmann {
template <> struct adl_serializer<CQuaternion> {
	static void to_json(json& j, const CQuaternion& p) { j = json{p.imag.x, p.imag.y, p.imag.z, p.real}; }

	static void from_json(const json& j, CQuaternion& p) {
		if (j.is_array() && j.size() == 4) {
			p.imag.x = j[0].get<float>();
			p.imag.y = j[1].get<float>();
			p.imag.z = j[2].get<float>();
			p.real = j[3].get<float>();
		} else {
			throw std::invalid_argument("CQuaternion must be an array of 4 floats");
		}
	}
};

} // namespace nlohmann