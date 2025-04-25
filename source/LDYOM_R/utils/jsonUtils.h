#pragma once
#include <CQuaternion.h>
#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <nlohmann/json.hpp>

namespace nlohmann {
	inline void jsonToUuid(const json &j, boost::uuids::uuid &uuid) {
		uuid = boost::uuids::string_generator()(j.get<std::string>());
	}

	inline json uuidToJson(const boost::uuids::uuid &uuid) {
		return boost::uuids::to_string(uuid);
	}

	template <typename T>
	json arrayPtrToJson(const T *array, const size_t size) {
		json j = json::array();
		for (size_t i = 0; i < size; i++) {
			j.push_back(array[i]);
		}
		return j;
	}

	template <typename T>
	void jsonToArrayPtr(const json &j, T *array, const size_t size) {
		for (size_t i = 0; i < size; i++) {
			j.at(i).get_to(array[i]);
		}
	}
}

NLOHMANN_JSON_NAMESPACE_BEGIN
	template <typename T>
	struct adl_serializer<std::unique_ptr<T>> {
		static void to_json(json &j, const std::unique_ptr<T> &up) {
			j = *up.get();
		}

		static void from_json(const json &j, std::unique_ptr<T> &p) {
			p = std::make_unique<T>(j.get<T>());
		}
	};

	template <>
	struct adl_serializer<CQuaternion> {
		static void to_json(json &j, const CQuaternion &p) {
			j = json{{"real", p.real}, {"imagX", p.imag.x}, {"imagY", p.imag.y}, {"imagZ", p.imag.z}};
		}

		static void from_json(const json &j, CQuaternion &p) {
			j.at("real").get_to(p.real);
			j.at("imagX").get_to(p.imag.x);
			j.at("imagY").get_to(p.imag.y);
			j.at("imagZ").get_to(p.imag.z);
		}
	};

	template <>
	struct adl_serializer<boost::uuids::uuid> {
		static void to_json(json &j, const boost::uuids::uuid &p) {
			j = uuidToJson(p);
		}

		static void from_json(const json &j, boost::uuids::uuid &p) {
			jsonToUuid(j, p);
		}
	};

NLOHMANN_JSON_NAMESPACE_END
