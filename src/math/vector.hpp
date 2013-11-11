#ifndef VECTOR_HPP
#define VECTOR_HPP

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
    union
    {
        float data[2];
        struct { float x, y; };
        struct { float r, g; };
        struct { float s, t; };
    };

public:
    vec2() = default;
    constexpr
    vec2(float x, float y) : data{x, y} {}

    float *ptr() { return data; }
    const float *ptr() const { return data; }
};


class vec3
{
public:
    union
    {
        float data[3];
        struct { float x, y, z; };
        struct { float r, g, b; };
        struct { float s, t, p; };

        // swizzle
        vec2 xy;
    };

public:
    vec3() = default;
    constexpr
    vec3(float x, float y, float z) : data{x, y, z} {}

    float *ptr() { return data; }
    const float *ptr() const { return data; }
};


class vec4
{
public:
    union
    {
        float data[4];
        struct { float x, y, z, w; };
        struct { float r, g, b, a; };
        struct { float s, t, p, q; };

        // swizzle
        vec3 xyz;
        vec2 xy;
    };

public:
    vec4() = default;
    constexpr
    vec4(float x, float y, float z, float w) : data{x, y, z, w} {}
    constexpr
    vec4(const vec3& v, float w) : data{v.x, v.y, v.z, w} {}

    float *ptr() { return data; }
    const float *ptr() const { return data; }
};


vec3 operator + (const vec3&, const vec3&);
vec3 operator - (const vec3&, const vec3&);
vec3 operator -(const vec3&);
vec3& operator *= (vec3&, float);
vec3 operator * (vec3, float);
vec3 operator * (float, vec3);

float dot(const vec3& l, const vec3& r);
float dot(const vec4& l, const vec4& r);
void norm3(vec3& V);
vec3 cross(const vec3& U, const vec3& V);
float mag(const vec3& v);

#endif // VECTOR_HPP
