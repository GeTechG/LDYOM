#pragma once
#include <CQuaternion.h>

class MathUtils {
public:
	static CVector vectorNormalize(const CVector &v);
	static CVector crossProduct(const CVector &v1, const CVector &v2);
	static float dotVector(const CVector &v1, const CVector &v2);
	static float dotQuaternion(const CQuaternion &q1, const CQuaternion &q2);
	static CQuaternion lookRotationQuat(const CVector &lookAt, const CVector &upDirection);
	static CMatrix LookAtRH(const CVector &eye, const CVector &target, const CVector &up);
	static std::array<float, 3> ToEulerAngles(CQuaternion &q);
	static CQuaternion matrixToQuat(const CMatrix &matrix);
	static CMatrix quatToMatrix(const CQuaternion &q);
	static void orthoNormalize(CVector &normal, CVector &tangent);
	static CQuaternion add(const CQuaternion &q1, const CQuaternion &q2);
	static CQuaternion subtract(const CQuaternion &q1, const CQuaternion &q2);
	static CQuaternion multiply(const CQuaternion &q, float a);
	static CVector multiply(const CVector &v1, const CVector &v2);
	static CVector multiply(const CMatrix &m, const CVector &v);
	static CQuaternion multiply(const CQuaternion &q1, const CQuaternion &q2);
	static float lerp(float x, float y, float a);
	static CQuaternion slerp(const CQuaternion &x, const CQuaternion &y, float a, int k = 1);
	static CMatrix matrixFromEular(float x, float y, float z);
};
