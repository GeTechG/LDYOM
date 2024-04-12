#include "CameraPath.h"

#include <boost/uuid/random_generator.hpp>

CameraPath::CameraPath(const char *name): uuid(boost::uuids::random_generator()()) {
	this->name = name;
	this->positionCurve.degree = 2;
}

CameraPath CameraPath::copy() const {
	CameraPath copy(*this);
	copy.uuid = boost::uuids::random_generator()();
	copy.name += " (copy)";
	return copy;
}

std::string& CameraPath::getName() {
	return name;
}

boost::uuids::uuid& CameraPath::getUuid() {
	return uuid;
}

tinynurbs::RationalCurve<float>& CameraPath::getPositionCurve() { return positionCurve; }
bool& CameraPath::isCustomControlKnots() { return customControlKnots; }
std::vector<CQuaternion>& CameraPath::getRotationsCurve() { return rotationsCurve; }
std::vector<int>& CameraPath::getRotationsEasing() { return rotationsEasing; }
float& CameraPath::getTime() { return time; }
bool& CameraPath::isCatmullRomRotations() { return catmullRomRotations; }

bool CameraPath::isValid() const {
	const bool isMoreTwoPoints = positionCurve.control_points.size() >= (catmullRomRotations ? 4 : 2);
	const bool isValidPointsCount = positionCurve.control_points.size() >= positionCurve.degree + 1;
	return curveIsValid(positionCurve) && isMoreTwoPoints && isValidPointsCount;
}
