#include "vector.hpp"

#include <cmath>

vec2::vec2()
: data{0, 0}
{}

vec2::vec2(float x, float y)
: data{x, y}
{}


vec3::vec3()
: data{0, 0, 0}
{}

vec3::vec3(float x, float y, float z)
: data{x, y, z}
{}


vec4::vec4()
: data{0, 0, 0, 1}
{}

vec4::vec4(float x, float y, float z, float w)
: data{x, y, z, w}
{}

vec4::vec4(const vec3& v, float w)
: data{v.x, v.y, v.z, w}
{}


vec3 operator - (const vec3& l, const vec3& r)
{
    return {l.x - r.x, l.y - r.y, l.z - r.z};
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
    const float s = 1.0f / sqrtf(dot(V, V));
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
