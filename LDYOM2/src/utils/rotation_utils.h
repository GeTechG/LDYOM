#pragma once

#include <CQuaternion.h>
#include <cmath>
#include <glm/glm.hpp>
#include <tuple>

/**
 * @brief Convert Euler angles (in degrees) to quaternion matching GTA SA's CMatrix::SetRotate(x,y,z).
 *
 * This formula was derived by analyzing GTA SA's CMatrix::SetRotate(float x, float y, float z)
 * and computing the equivalent quaternion that produces the same rotation matrix when
 * converted via CMatrix::SetRotate(RtQuat*).
 *
 * Note: GTA SA's CQuaternion::Set uses a DIFFERENT Euler convention than CMatrix::SetRotate,
 * so we cannot use CQuaternion::Set directly. This function implements the correct ZXY
 * Euler convention that matches CMatrix::SetRotate(x,y,z).
 *
 * DYOM uses Euler angles (rotation_x, rotation_y, rotation_z) in degrees via opcode 0453.
 * LDYOM uses quaternions [x, y, z, w].
 *
 * @param rotXDeg X rotation in degrees
 * @param rotYDeg Y rotation in degrees
 * @param rotZDeg Z rotation in degrees
 * @return CQuaternion Quaternion [w, x, y, z] (CQuaternion uses w-first convention internally)
 */
inline CQuaternion eulerToQuaternion(float rotXDeg, float rotYDeg, float rotZDeg) {
	// Convert degrees to radians
	float x = glm::radians(rotXDeg);
	float y = glm::radians(rotYDeg);
	float z = glm::radians(rotZDeg);

	// Calculate half angles and their sin/cos
	float cx2 = std::cos(x / 2.0f);
	float sx2 = std::sin(x / 2.0f);
	float cy2 = std::cos(y / 2.0f);
	float sy2 = std::sin(y / 2.0f);
	float cz2 = std::cos(z / 2.0f);
	float sz2 = std::sin(z / 2.0f);

	// ZXY Euler convention quaternion formula
	// Derived from GTA SA's CMatrix::SetRotate(x,y,z) matrix formula
	float w = cz2 * cx2 * cy2 - sz2 * sx2 * sy2;
	float qx = cz2 * sx2 * cy2 - sz2 * cx2 * sy2;
	float qy = cz2 * cx2 * sy2 + sz2 * sx2 * cy2;
	float qz = sz2 * cx2 * cy2 + cz2 * sx2 * sy2;

	// CQuaternion constructor is (w, x, y, z)
	return {{qx, qy, qz}, w};
}

/**
 * @brief Convert quaternion to Euler angles (in degrees) matching GTA SA's CMatrix::SetRotate(x,y,z).
 *
 * This is the INVERSE of eulerToQuaternion - converts quaternions back to Euler angles
 * using the same ZXY convention.
 *
 * LDYOM uses quaternions [x, y, z, w].
 * This function converts them to Euler angles (rotation_x, rotation_y, rotation_z) in degrees.
 *
 * @param q Quaternion (GLM uses w-first convention: q.w, q.x, q.y, q.z)
 * @return std::tuple<float, float, float> Tuple of (rotXDeg, rotYDeg, rotZDeg) in degrees
 *
 * Note: Due to gimbal lock at X ≈ ±90°, the returned angles may differ from the original
 * input angles while still representing the same rotation (they will produce an equivalent
 * rotation matrix).
 */
inline std::array<float, 3> quaternionToEuler(const CQuaternion& q) {
	float qx = q.imag.x;
	float qy = q.imag.y;
	float qz = q.imag.z;
	float qw = q.real;

	// First convert quaternion to rotation matrix (using GTA SA's formula)
	float x2 = qx + qx;
	float y2 = qy + qy;
	float z2 = qz + qz;

	float xx = x2 * qx;
	float xy = y2 * qx;
	float xz = z2 * qx;

	float yy = y2 * qy;
	float yz = z2 * qy;

	float zz = z2 * qz;

	float wx = x2 * qw;
	float wy = y2 * qw;
	float wz = z2 * qw;

	// Matrix components needed for angle extraction
	float right_z = xz - wy;
	float top_x = xy - wz;
	float top_y = 1.0f - (zz + xx);
	float top_z = wx + yz;
	float at_z = 1.0f - (yy + xx);

	// Extract Euler angles from matrix using ZXY convention
	// From GTA SA's CMatrix::SetRotate(x, y, z):
	//   top.z = sin(x)
	//   right.z = -sin(y) * cos(x)
	//   at.z = cos(y) * cos(x)
	//   top.x = -sin(z) * cos(x)
	//   top.y = cos(z) * cos(x)

	// Extract X rotation
	float sin_x = top_z;
	// Clamp to [-1, 1] to avoid numerical errors with asin
	sin_x = glm::clamp(sin_x, -1.0f, 1.0f);
	float x = std::asin(sin_x);

	// Check for gimbal lock
	float cos_x = std::cos(x);

	float y, z;
	if (std::abs(cos_x) > 0.00001f) { // Not in gimbal lock
		// Extract Y and Z rotations
		// y = atan2(-right.z / cos(x), at.z / cos(x))
		// z = atan2(-top.x / cos(x), top.y / cos(x))
		y = std::atan2(-right_z / cos_x, at_z / cos_x);
		z = std::atan2(-top_x / cos_x, top_y / cos_x);
	} else {
		// Gimbal lock case (x ≈ ±90°)
		// When x = ±90°, matrix becomes:
		//   x = 90°:  right = (cos(z+y), sin(z+y), 0), top = (0, 0, 1), at = (sin(z+y), -cos(z+y), 0)
		//   x = -90°: right = (cos(z-y), sin(z-y), 0), top = (0, 0, -1), at = (-sin(z-y), cos(z-y), 0)
		// We can only recover z+y (or z-y), so we set y=0 and z=z+y (or z=z-y)
		float right_x = 1.0f - (zz + yy);
		float right_y = xy + wz;

		// For both cases, z = atan2(right.y, right.x)
		// When x=90°: z = z+y, setting y=0 gives z
		// When x=-90°: z = z-y, setting y=0 gives z
		y = 0.0f;
		z = std::atan2(right_y, right_x);
	}

	// Convert to degrees
	return {glm::degrees(x), glm::degrees(y), glm::degrees(z)};
}
