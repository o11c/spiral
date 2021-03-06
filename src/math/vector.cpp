#include "vector.hpp"

#include <cmath>

vec3 operator + (const vec3& l, const vec3& r)
{
    return {l.x + r.x, l.y + r.y, l.z + r.z};
}
vec3& operator += (vec3& l, const vec3& r)
{
    return l = l + r;
}
vec3 operator - (const vec3& l, const vec3& r)
{
    return {l.x - r.x, l.y - r.y, l.z - r.z};
}
vec3& operator -= (vec3& l, const vec3& r)
{
    return l = l - r;
}
vec3 operator -(const vec3& l)
{
    return {-l.x, -l.y, -l.z};
}

vec3& operator *= (vec3& V, float s)
{
    V.x *= s;
    V.y *= s;
    V.z *= s;
    return V;
}
vec3 operator * (vec3 v, float s)
{
    v *= s;
    return v;
}
vec3 operator * (float s, vec3 v)
{
    v *= s;
    return v;
}

float dot(const vec3& l, const vec3& r)
{
    return l.x * r.x + l.y * r.y + l.z * r.z;
}
float dot(const vec4& l, const vec4& r)
{
    return l.x * r.x + l.y * r.y + l.z * r.z + l.w * r.w;
}

void norm3(vec3& V)
{
    const float s = 1.0f / mag(V);
    V *= s;
}

vec3 cross(const vec3& U, const vec3& V)
{
    return {
        U.y * V.z - U.z * V.y,
        U.z * V.x - U.x * V.z,
        U.x * V.y - U.y * V.x,
    };
}

float mag(const vec3& v)
{
    return sqrtf(dot(v, v));
}
