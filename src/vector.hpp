#ifndef VECTOR_HPP
#define VECTOR_HPP

#include "property.hpp"

struct ivec2
{
    __attribute__((error("NYI")))
    const int *ptr() const;
};
struct ivec3
{
    __attribute__((error("NYI")))
    const int *ptr() const;
};
struct ivec4
{
    __attribute__((error("NYI")))
    const int *ptr() const;
};

class vec2
{
public:
    PROPERTY(vec2, float, x) { return data[0]; };
    PROPERTY(vec2, float, y) { return data[1]; };

    PROPERTY(vec2, float, r) { return data[0]; };
    PROPERTY(vec2, float, g) { return data[1]; };

    PROPERTY(vec2, float, A) { return data[0]; };
    PROPERTY(vec2, float, B) { return data[1]; };
private:
    float data[2];
public:
    vec2();
    vec2(float, float);

    float *ptr() { return data; }
    const float *ptr() const { return data; }
};


class vec3
{
public:
    PROPERTY(vec3, float, x) { return data[0]; };
    PROPERTY(vec3, float, y) { return data[1]; };
    PROPERTY(vec3, float, z) { return data[2]; };

    PROPERTY(vec3, float, r) { return data[0]; };
    PROPERTY(vec3, float, g) { return data[1]; };
    PROPERTY(vec3, float, b) { return data[2]; };

    PROPERTY(vec3, float, A) { return data[0]; };
    PROPERTY(vec3, float, B) { return data[1]; };
    PROPERTY(vec3, float, C) { return data[2]; };
private:
    float data[3];
public:
    vec3();
    vec3(float, float, float);

    float *ptr() { return data; }
    const float *ptr() const { return data; }
};


class vec4
{
public:
    PROPERTY(vec4, float, x) { return data[0]; };
    PROPERTY(vec4, float, y) { return data[1]; };
    PROPERTY(vec4, float, z) { return data[2]; };
    PROPERTY(vec4, float, w) { return data[3]; };

    PROPERTY(vec4, float, r) { return data[0]; };
    PROPERTY(vec4, float, g) { return data[1]; };
    PROPERTY(vec4, float, b) { return data[2]; };
    PROPERTY(vec4, float, a) { return data[3]; };

    PROPERTY(vec4, float, s) { return data[0]; };
    PROPERTY(vec4, float, t) { return data[1]; };
    PROPERTY(vec4, float, p) { return data[2]; };
    PROPERTY(vec4, float, q) { return data[3]; };

    PROPERTY(vec4, float, A) { return data[0]; };
    PROPERTY(vec4, float, B) { return data[1]; };
    PROPERTY(vec4, float, C) { return data[2]; };
    PROPERTY(vec4, float, D) { return data[3]; };
private:
    float data[4];
public:
    vec4();
    vec4(float, float, float, float);
    vec4(const vec3& v, float w);

    float *ptr() { return data; }
    const float *ptr() const { return data; }
};


vec3 operator - (const vec3&, const vec3&);
vec3 operator -(const vec3&);
vec3& operator *= (vec3&, float);

float dot(const vec3& l, const vec3& r);
float dot(const vec4& l, const vec4& r);
void norm3(vec3& V);
vec3 cross(const vec3& U, const vec3& V);

#endif // VECTOR_HPP
