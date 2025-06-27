#pragma once
#include <CMatrix.h>

inline void scaleMatrix(CMatrix& matrix, const std::array<float, 3>& scale) {
	matrix.right.x *= scale[0];
	matrix.right.y *= scale[0];
	matrix.right.z *= scale[0];
	matrix.up.x *= scale[1];
	matrix.up.y *= scale[1];
	matrix.up.z *= scale[1];
	matrix.at.x *= scale[2];
	matrix.at.y *= scale[2];
	matrix.at.z *= scale[2];
	matrix.UpdateRW();
}