#include <sol.hpp>

#include "../Data/CameraPath.h"

void cameraPathWrapper(sol::state &state) {
	state.new_usertype<CameraPath>("LDCameraPath",
	                               sol::no_constructor,
	                               "getName", &CameraPath::getName,
	                               "copy", &CameraPath::copy,
	                               "getName", &CameraPath::getName,
	                               "getUuid", &CameraPath::getUuid,
	                               "getPositionCurve", &CameraPath::getPositionCurve,
	                               "isCustomControlKnots", &CameraPath::isCustomControlKnots,
	                               "getRotationsCurve", &CameraPath::getRotationsCurve,
	                               "getRotationsEasing", &CameraPath::getRotationsEasing,
	                               "getTime", &CameraPath::getTime,
	                               "isCatmullRomRotations", &CameraPath::isCatmullRomRotations,
	                               "isValid", &CameraPath::isValid
	);
}
