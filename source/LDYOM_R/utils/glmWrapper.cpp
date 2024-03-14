#include <sol.hpp>

#include "glm/fwd.hpp"
#include "glm/detail/type_quat.hpp"
#include "glm/gtc/quaternion.hpp"

void glmWrapper(sol::state &state) {
	auto glmTable = state.create_table("glm");
	glmTable.new_usertype<glm::vec3>("vec3",
	                                 sol::constructors<glm::vec3(), glm::vec3(float, float, float)>(),
	                                 "x", &glm::vec3::x,
	                                 "y", &glm::vec3::y,
	                                 "z", &glm::vec3::z
	);
	glmTable.new_usertype<glm::quat>("quat",
	                                 sol::constructors<glm::quat(), glm::quat(float, float, float, float)>(),
	                                 "x", &glm::quat::x,
	                                 "y", &glm::quat::y,
	                                 "z", &glm::quat::z,
	                                 "w", &glm::quat::w
	);
	glmTable["eulerAngles"] = [](const glm::quat &quat) {
		return eulerAngles(quat);
	};
	glmTable["degrees"] = [](const float radians) {
		return glm::degrees(radians);
	};
	glmTable["degreesVec3"] = [](const glm::vec3 &eulerAngles) {
		return degrees(eulerAngles);
	};
	glmTable["radians"] = [](const float degrees) {
		return glm::radians(degrees);
	};
	glmTable["quatLookAtLH"] = [](const glm::vec3 &direction, const glm::vec3 &up) -> glm::quat {
		return quatLookAtLH(direction, up);
	};
	glmTable["angleAxis"] = [](const float angle, const glm::vec3 &axis) -> glm::quat {
		return angleAxis(angle, axis);
	};
	glmTable["multiplyVec3"] = [](const glm::quat &q1, const glm::vec3 &v) {
		return q1 * v;
	};
	glmTable["multiply"] = [](const glm::quat &q1, const glm::quat &q2) {
		return q1 * q2;
	};
	glmTable["normalize"] = [](const glm::quat &q) {
		return normalize(q);
	};
}
