#pragma once
#include <nlohmann/json.hpp>
#include "glmSerialization.h"
#include "tinynurbs/core/curve.h"

NLOHMANN_JSON_NAMESPACE_BEGIN
	template <typename T>
	struct adl_serializer<tinynurbs::RationalCurve<T>> {
		static void to_json(json &j, const tinynurbs::RationalCurve<T> &obj) {
			j["degree"] = obj.degree;
			j["knots"] = obj.knots;
			j["control_points"] = obj.control_points;
			j["weights"] = obj.weights;
		}

		static void from_json(const json &j, tinynurbs::RationalCurve<T> &obj) {
			j.at("degree").get_to(obj.degree);
			j.at("knots").get_to(obj.knots);
			j.at("control_points").get_to(obj.control_points);
			j.at("weights").get_to(obj.weights);
		}
	};

NLOHMANN_JSON_NAMESPACE_END
