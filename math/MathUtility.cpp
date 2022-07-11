#include "MathUtility.h"

float MathUtility::Radian(float radian)
{
    return radian * PI / 180;
}

Vector3 MathUtility::Vector3MultiTransform(const Vector3& v, const Matrix4& m)
{
    Vector3 vec3{};

    vec3.x = (v.x * m.m[0][0]) + (v.y * m.m[1][0]) + (v.z * m.m[2][0]);
    vec3.y = (v.x * m.m[0][1]) + (v.y * m.m[1][1]) + (v.z * m.m[2][1]);
    vec3.z = (v.x * m.m[0][2]) + (v.y * m.m[1][2]) + (v.z * m.m[2][2]);

    return vec3;

}
