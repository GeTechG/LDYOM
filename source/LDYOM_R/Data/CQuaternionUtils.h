#pragma once
#include <CQuaternion.h>

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

inline glm::quat CQuatToGlmQuat(const CQuaternion &quat) {
	return {quat.real, quat.imag.x, quat.imag.y, quat.imag.z};
}

inline glm::quat catmullRom(const glm::quat &p0, const glm::quat &p1, const glm::quat &p2, const glm::quat &p3,
                            float t) {
	const float t2 = t * t;
	const float t3 = t2 * t;

	const float w0 = -0.5f * t3 + t2 - 0.5f * t;
	const float w1 = 1.5f * t3 - 2.5f * t2 + 1.0f;
	const float w2 = -1.5f * t3 + 2.0f * t2 + 0.5f * t;
	const float w3 = 0.5f * t3 - 0.5f * t2;

	return p0 * w0 + p1 * w1 + p2 * w2 + p3 * w3;
}

inline CQuaternion catmullRomSpline(const std::vector<CQuaternion> &q, float t) {
	const int numPoints = q.size();
	const int p = static_cast<int>(t * static_cast<float>(numPoints - 1));
	t = t * static_cast<float>(numPoints - 1) - static_cast<float>(p);

	const glm::quat p0 = CQuatToGlmQuat(q[glm::clamp(p - 1, 0, numPoints - 1)]);
	const glm::quat p1 = CQuatToGlmQuat(q[p]);
	const glm::quat p2 = CQuatToGlmQuat(q[glm::clamp(p + 1, 0, numPoints - 1)]);
	const glm::quat p3 = CQuatToGlmQuat(q[glm::clamp(p + 2, 0, numPoints - 1)]);

	auto quat = catmullRom(p0, p1, p2, p3, t);
	return {{quat.x, quat.y, quat.z}, quat.w};
}

inline void ensureShortestPath(std::vector<CQuaternion> &quaternions) {
	for (size_t i = 1; i < quaternions.size(); ++i) {
		if (dot(CQuatToGlmQuat(quaternions[i - 1]), CQuatToGlmQuat(quaternions[i])) < 0) {
			quaternions[i] *= -1;
		}
	}
}
