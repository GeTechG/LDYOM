#pragma once
#define UUID_SYSTEM_GENERATOR 1
#include "nlohmann/json.hpp"
#include <uuid.h>

namespace nlohmann {
template <> struct adl_serializer<uuids::uuid> {
	static void to_json(json& j, const uuids::uuid& id) { j = uuids::to_string(id); }

	static void from_json(const json& j, uuids::uuid& id) {
		if (j.is_string()) {
			auto opt_id = uuids::uuid::from_string(j.get<std::string>());
			if (opt_id) {
				id = *opt_id;
			} else {
				throw std::runtime_error("Invalid UUID string format in JSON");
			}
		} else {
			throw std::runtime_error("Expected string for UUID deserialization");
		}
	}
};
} // namespace nlohmann