#include "MathUtils.h"

#include <array>
#include <CMatrix.h>

#define PI 3.14159265358979323846f

CVector MathUtils::vectorNormalize(const CVector& v) {
    CVector newVector = v;
    newVector.Normalise();
    return newVector;
}

CVector MathUtils::crossProduct(const CVector& v1, const CVector& v2) {
    CVector newVector;
    newVector.Cross(v1, v2);
    return newVector;
}

float MathUtils::dotVector(const CVector& v1, const CVector& v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float MathUtils::dotQuaternion(const CQuaternion& q1, const CQuaternion& q2) {
    return q1.imag.x * q2.imag.x + q1.imag.y * q2.imag.y + q1.imag.z * q2.imag.z + q1.real * q2.real;
}

CQuaternion MathUtils::lookRotationQuat(const CVector& lookAt, const CVector& upDirection) {

	CVector forward = lookAt;
    CVector up = upDirection;

    forward.Normalise();
    up = up - forward * dotVector(up, forward);
    up.Normalise();

	const CVector vector = vectorNormalize(forward);
	const CVector vector2 = crossProduct(up, vector);
	const CVector vector3 = crossProduct(vector, vector2);
	const float m00 = vector2.x;
	const float m01 = vector2.y;
	const float m02 = vector2.z;
	const float m10 = vector3.x;
	const float m11 = vector3.y;
	const float m12 = vector3.z;
	const float m20 = vector.x;
	const float m21 = vector.y;
	const float m22 = vector.z;


	const float num8 = m00 + m11 + m22;
    CQuaternion quaternion;
    if (num8 > 0.0f) {
        float num = sqrtf(num8 + 1.0f);
        quaternion.real = num * 0.5f;
        num = 0.5f / num;
        quaternion.imag.x = (m12 - m21) * num;
        quaternion.imag.y = (m20 - m02) * num;
        quaternion.imag.z = (m01 - m10) * num;
        return quaternion;
    }
    if (m00 >= m11 && m00 >= m22) {
	    const float num7 = sqrtf(1.0f + m00 - m11 - m22);
	    const float num4 = 0.5f / num7;
        quaternion.imag.x = 0.5f * num7;
        quaternion.imag.y = (m01 + m10) * num4;
        quaternion.imag.z = (m02 + m20) * num4;
        quaternion.real = (m12 - m21) * num4;
        return quaternion;
    }
    if (m11 > m22) {
	    const float num6 = sqrtf(1.0f + m11 - m00 - m22);
	    const float num3 = 0.5f / num6;
        quaternion.imag.x = (m10 + m01) * num3;
        quaternion.imag.y = 0.5f * num6;
        quaternion.imag.z = (m21 + m12) * num3;
        quaternion.real = (m20 - m02) * num3;
        return quaternion;
    }

	const float num5 = sqrtf(1.0f + m22 - m00 - m11);
	const float num2 = 0.5f / num5;
    quaternion.imag.x = (m20 + m02) * num2;
    quaternion.imag.y = (m21 + m12) * num2;
    quaternion.imag.z = 0.5f * num5;
    quaternion.real = (m01 - m10) * num2;

    return quaternion;
}

CMatrix MathUtils::LookAtRH(const CVector& eye, const CVector& target, const CVector& up) {
	const CVector zaxis = vectorNormalize((eye - target));    // The "forward" vector.
	const CVector xaxis = vectorNormalize(crossProduct(up, zaxis));// The "right" vector.
	const CVector yaxis = crossProduct(zaxis, xaxis);     // The "up" vector.

	// Create a 4x4 view matrix from the right, up, forward and eye position vectors
	RwMatrix viewMatrix = {
        {   xaxis.x, yaxis.x,zaxis.x}, 0,
          { xaxis.y, yaxis.y, zaxis.y}, 0,
          { xaxis.z, yaxis.z, zaxis.z}, 0,
         {-dotVector(xaxis, eye), -dotVector(yaxis, eye), -dotVector(zaxis, eye)},  1
	};

	return {&viewMatrix, true};
}

std::array<float, 3> MathUtils::ToEulerAngles(CQuaternion& q) {
    std::array<float, 3> angles{};

    const float sqw = q.real * q.real;
    const float sqx = q.imag.x * q.imag.x;
    const float sqy = q.imag.y * q.imag.y;
    const float sqz = q.imag.z * q.imag.z;
    const float unit = sqx + sqy + sqz + sqw; // if normalised is one, otherwise is correction factor
    const float test = q.imag.x * q.imag.y + q.imag.z * q.real;
    if (test > 0.499f * unit) { // singularity at north pole
        angles[1] = 2 * atan2f(q.imag.x, q.real);
        angles[0] = PI / 2;
        angles[2] = 0;
        return angles;
    }
    if (test < -0.499f * unit) { // singularity at south pole
        angles[1] = -2 * atan2f(q.imag.x, q.real);
        angles[0] = -PI / 2;
        angles[2] = 0;
        return angles;
    }
    angles[1] = atan2f(2.f * q.imag.y * q.real - 2.f * q.imag.x * q.imag.z, sqx - sqy - sqz + sqw);
    angles[0] = asinf(2.f * test / unit);
    angles[2] = atan2f(2.f * q.imag.x * q.real - 2.f * q.imag.y * q.imag.z, -sqx + sqy - sqz + sqw);
    return angles;
}

CQuaternion MathUtils::matrixToQuat(const CMatrix& matrix) {
    CQuaternion q;
    const float trace = matrix.right.x + matrix.up.y + matrix.at.z + 1.f; // I removed + 1.0f; see discussion with Ethan
    if (trace > 0.0001f) {// I changed M_EPSILON to 0
        const float s = sqrtf(trace) * 2.f;
        q.real = 0.25f * s;
        q.imag.x = (matrix.at.y - matrix.up.z) / s;
        q.imag.y = (matrix.right.z - matrix.at.x) / s;
        q.imag.z = (matrix.up.x - matrix.right.y) / s;
    }
    else {
        if (matrix.right.x > matrix.up.y && matrix.right.x > matrix.at.z) {
            const float s = 2.0f * sqrtf(1.0f + matrix.right.x - matrix.up.y - matrix.at.z);
            q.real = (matrix.at.y - matrix.up.z) / s;
            q.imag.x = 0.25f * s;
            q.imag.y = (matrix.right.y + matrix.up.x) / s;
            q.imag.z = (matrix.right.z + matrix.at.x) / s;
        }
        else if (matrix.up.y > matrix.at.z) {
            const float s = 2.0f * sqrtf(1.0f + matrix.up.y - matrix.right.x - matrix.at.z);
            q.real = (matrix.right.z - matrix.at.x) / s;
            q.imag.x = (matrix.right.y + matrix.up.x) / s;
            q.imag.y = 0.25f * s;
            q.imag.z = (matrix.up.z + matrix.at.y) / s;
        }
        else {
            const float s = 2.0f * sqrtf(1.0f + matrix.at.z - matrix.right.x - matrix.up.y);
            q.real = (matrix.up.x - matrix.right.y) / s;
            q.imag.x = (matrix.right.z + matrix.at.x) / s;
            q.imag.y = (matrix.up.z + matrix.at.y) / s;
            q.imag.z = 0.25f * s;
        }
    }
    return q;
}

CMatrix MathUtils::quatToMatrix(const CQuaternion& q) {
    CMatrix m;

    const float sqw = q.real * q.real;
    const float sqx = q.imag.x * q.imag.x;
    const float sqy = q.imag.y * q.imag.y;
    const float sqz = q.imag.z * q.imag.z;

    // invs (inverse square length) is only required if quaternion is not already normalised
    const float invs = 1.0 / (sqx + sqy + sqz + sqw);
    m.right.x = (sqx - sqy - sqz + sqw) * invs; // since sqw + sqx + sqy + sqz =1/invs*invs
    m.up.y = (-sqx + sqy - sqz + sqw) * invs;
    m.at.z = (-sqx - sqy + sqz + sqw) * invs;

    float tmp1 = q.imag.x * q.imag.y;
    float tmp2 = q.imag.z * q.real;

    m.up.x = 2.f * (tmp1 + tmp2) * invs;
    m.right.y = 2.f * (tmp1 - tmp2) * invs;

    tmp1 = q.imag.x * q.imag.z;
    tmp2 = q.imag.y * q.real;

    m.at.x = 2.f * (tmp1 - tmp2) * invs;
    m.right.z = 2.f * (tmp1 + tmp2) * invs;

    tmp1 = q.imag.y * q.imag.z;
    tmp2 = q.imag.x * q.real;

    m.at.y = 2.f * (tmp1 + tmp2) * invs;
    m.up.z = 2.f * (tmp1 - tmp2) * invs;

    return m;
}

void MathUtils::orthoNormalize(CVector& normal, CVector& tangent) {
	normal.Normalise();
	tangent.Normalise();
	tangent.Cross(normal, tangent);
}

CQuaternion MathUtils::add(const CQuaternion& q1, const CQuaternion& q2) {
    auto r = q1;
    r += q2;
    return r;
}

CQuaternion MathUtils::subtract(const CQuaternion& q1, const CQuaternion& q2) {
    auto r = q1;
    r -= q2;
    return r;
}

CQuaternion MathUtils::multiply(const CQuaternion& q, const float a) {
    auto r = q;
    r *= a;
    return r;
}

CVector MathUtils::multiply(const CVector& v1, const CVector& v2) {
    return {v1.x * v2.x, v1.y * v2.y, v1.z * v2.z};
}

CVector MathUtils::multiply(const CMatrix& m, const CVector& v) {
    return {
    	m.right.x* v.x + m.right.y * v.y + m.right.z * v.z,
        m.up.x* v.x + m.up.y * v.y + m.up.z * v.z,
        m.at.x* v.x + m.at.y * v.y + m.at.z * v.z
    };
}

CQuaternion MathUtils::multiply(const CQuaternion& q1, const CQuaternion& q2) {
    CQuaternion r;
	r.Multiply(q1, q2);
    return r;
}

float MathUtils::lerp(const float x, const float y, const float a) {
    return x * (1.f - a) + y * a;
}

CQuaternion MathUtils::slerp(const CQuaternion& x, const CQuaternion& y, const float a, const int k) {
    auto z = y;

    auto cosTheta = dotQuaternion(x, y);

    // If cosTheta < 0, the interpolation will take the long way around the sphere.
    // To fix this, one quat must be negated.
    if (cosTheta < 0.f) {
        z *= -1;
        cosTheta = -cosTheta;
    }
    
    // Perform a linear interpolation when cosTheta is close to 1 to avoid side effect of sin(angle) becoming a zero denominator
    if (cosTheta > 1.f - FLT_EPSILON) {
        // Linear interpolation
        return {
        	{
        		lerp(x.real, z.real, a),
        		lerp(x.imag.x, z.imag.x, a),
        		lerp(x.imag.y, z.imag.y, a)
        	},
            lerp(x.imag.z, z.imag.z, a)};
    }

    // Graphics Gems III, page 96
    const auto angle = acos(cosTheta);
    const auto phi = angle + static_cast<float>(k) * PI;
    return multiply(add(multiply(x, sin(angle - a * phi)), multiply(z, sin(a * phi))), pow(sin(angle), -1.f));
}

CMatrix MathUtils::matrixFromEular(const float x, const float y, const float z) {
    // Assuming the angles are in radians.
    const float ch = cos(y);
    const float sh = sin(y);
    const float ca = cos(x);
    const float sa = sin(x);
    const float cb = cos(z);
    const float sb = sin(z);

    CMatrix m;

    m.right.x = ch * ca;
    m.right.y = sh * sb - ch * sa * cb;
    m.right.z = ch * sa * sb + sh * cb;
    m.up.x = sa;
    m.up.y = ca * cb;
    m.up.z = -ca * sb;
    m.at.x = -sh * ca;
    m.at.y = sh * sa * cb + ch * sb;
    m.at.z = -sh * sa * sb + ch * cb;

    return m;
}
