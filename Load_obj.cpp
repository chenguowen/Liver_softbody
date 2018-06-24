#include "LoadObj.h"




inline bool operator == (const float2 &a, const float2 &b)
{
	if (fabs(a.x - b.x) > MATH_EPSILON) return false;
	if (fabs(a.y - b.y) > MATH_EPSILON) return false;
	return true;
}



inline float3 normalize(const float3 &v)
{
	float l = (v.x*v.x)+(v.y*v.y)+(v.z*v.z);
	if (l < MATH_EPSILON)
		return float3(0.0, 0.0, 0.0);

	return v * (1.0f / sqrtf(l));
}

inline float3 cross(const float3 &a, const float3 &b)
{
	return float3(a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x);
}

inline float dot(const float3 &a, const float3 &b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

inline bool operator == (const float3 &a, const float3 &b)
{
	if (fabs(a.x - b.x) > MATH_EPSILON) return false;
	if (fabs(a.y - b.y) > MATH_EPSILON) return false;
	if (fabs(a.z - b.z) > MATH_EPSILON) return false;
	return true;
}



inline bool operator == (const float4 &a, const float4 &b)
{
	if (fabs(a.x - b.x) > MATH_EPSILON) return false;
	if (fabs(a.y - b.y) > MATH_EPSILON) return false;
	if (fabs(a.z - b.z) > MATH_EPSILON) return false;
	if (fabs(a.w - b.w) > MATH_EPSILON) return false;
	return true;
}
