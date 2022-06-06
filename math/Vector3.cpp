#include "Vector3.h"
#include<cmath>

Vector3& Vector3::Normalize()
{
	float len = sqrt(x * x + y * y + z * z);
	if (len != 0) {
		return *this /= len;
	}
	return *this;
}

Vector3 Vector3::Cross(const Vector3& v) const
{
	return Vector3(
		y * v.z - z * v.y,
		z * v.x - x * v.z,
		x * v.y - y * v.x
	);
}
