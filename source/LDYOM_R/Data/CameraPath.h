#pragma once
#include <array>
#include <CQuaternion.h>
#include <nlohmann/json.hpp>

#include "INameable.h"
#include "IUuidable.h"
#include "jsonUtils.h"
#include "rationalCurveSerialize.h"
#include "tinynurbs/tinynurbs.h"

struct CameraPathPoint {
	std::array<float, 3> pos{};
	CQuaternion rotate{};
};


class CameraPath : public INameable, public IUuidable {
private:
	boost::uuids::uuid uuid;

	std::string name{};
	std::vector<CQuaternion> rotationsCurve;
	std::vector<int> rotationsEasing;
	tinynurbs::RationalCurve<float> positionCurve;
	bool customControlKnots = false;
	float time = 1.0f;
	bool catmullRomRotations = true;

public:
	CameraPath() = default;
	CameraPath(const char *name);
	CameraPath(const CameraPath &other) = default;
	CameraPath& operator=(const CameraPath &other) = default;

	CameraPath copy() const;

	std::string& getName() override;
	boost::uuids::uuid& getUuid() override;

	tinynurbs::RationalCurve<float>& getPositionCurve();
	bool& isCustomControlKnots();
	std::vector<CQuaternion>& getRotationsCurve();
	std::vector<int>& getRotationsEasing();
	float& getTime();
	bool& isCatmullRomRotations();

	bool isValid() const;
};

NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<CameraPath> {
		static void to_json(json &j, const CameraPath &obj) {
			auto &a = const_cast<CameraPath&>(obj);
			j["uuid"] = a.getUuid();
			j["name"] = a.getName();
			j["rotationsCurve"] = a.getRotationsCurve();
			j["positionCurve"] = a.getPositionCurve();
			j["customControlKnots"] = a.isCustomControlKnots();
			j["time"] = a.getTime();
			j["rotationsEasing"] = a.getRotationsEasing();
			j["catmullRomRotations"] = a.isCatmullRomRotations();
		}

		static void from_json(const json &j, CameraPath &obj) {
			j.at("uuid").get_to(obj.getUuid());
			j.at("name").get_to(obj.getName());
			j.at("rotationsCurve").get_to(obj.getRotationsCurve());
			j.at("positionCurve").get_to(obj.getPositionCurve());
			j.at("customControlKnots").get_to(obj.isCustomControlKnots());
			j.at("time").get_to(obj.getTime());
			j.at("rotationsEasing").get_to(obj.getRotationsEasing());
			j.at("catmullRomRotations").get_to(obj.isCatmullRomRotations());
		}
	};

NLOHMANN_JSON_NAMESPACE_END
