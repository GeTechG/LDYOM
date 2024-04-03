#pragma once
#include <CVector.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifndef CMatrixUtils_H
#define CMatrixUtils_H

inline void ConvertToEulerAngles(const CMatrix &matrix, float *pX, float *pY, float *pZ, const UINT32 uiFlags) {
	plugin::CallMethod<0x59A840, const CMatrix*, float*, float*, float*, UINT32>(&matrix, pX, pY, pZ, uiFlags);
}

inline void ConvertFromEulerAngles(CMatrix &matrix, const float x, const float y, const float z, const UINT32 uiFlags) {
	plugin::CallMethod<0x59AA40, CMatrix*, float, float, float, UINT32>(&matrix, x, y, z, uiFlags);
}

inline glm::mat4 CMatrixToGlmMat4(const CMatrix &cMatrix) {
	glm::mat4 result(1.0f);

	result[0].x = cMatrix.right.x;
	result[0].y = cMatrix.right.y;
	result[0].z = cMatrix.right.z;

	result[1].x = cMatrix.up.x;
	result[1].y = cMatrix.up.y;
	result[1].z = cMatrix.up.z;

	result[2].x = cMatrix.at.x;
	result[2].y = cMatrix.at.y;
	result[2].z = cMatrix.at.z;

	result[3].x = cMatrix.pos.x;
	result[3].y = cMatrix.pos.y;
	result[3].z = cMatrix.pos.z;

	return result;
}

inline CMatrix GlmMat4ToCMatrix(const glm::mat4 &mat) {
	CMatrix result;

	result.right.x = mat[0].x;
	result.right.y = mat[0].y;
	result.right.z = mat[0].z;

	result.up.x = mat[1].x;
	result.up.y = mat[1].y;
	result.up.z = mat[1].z;

	result.at.x = mat[2].x;
	result.at.y = mat[2].y;
	result.at.z = mat[2].z;

	result.pos.x = mat[3].x;
	result.pos.y = mat[3].y;
	result.pos.z = mat[3].z;

	return result;
}

#endif // CMatrixUtils_H
