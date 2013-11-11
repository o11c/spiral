#ifndef VECTOR_HPP
#define VECTOR_HPP

class ivec2
{
public:
    union
    {
        int data[2];
        struct { int x, y; };
        struct { int r, g; };
        struct { int s, t; };
    };

public:
    ivec2() = default;
    constexpr
    ivec2(int x, int y) : data{x, y} {}

    int *ptr() { return data; }
    const int *ptr() const { return data; }
};

class ivec3
{
public:
    union
    {
        int data[3];
        struct { int x, y, z; };
        struct { int r, g, b; };
        struct { int s, t, p; };

        // swizzle
        ivec2 xy;
    };

public:
    ivec3() = default;
    constexpr
    ivec3(int x, int y, int z) : data{x, y, z} {}

    int *ptr() { return data; }
    const int *ptr() const { return data; }
};

class ivec4
{
public:
    union
    {
        int data[4];
        struct { int x, y, z, w; };
        struct { int r, g, b, a; };
        struct { int s, t, p, q; };

        // swizzle
        ivec3 xyz;
        ivec2 xy;
    };

public:
    ivec4() = default;
    constexpr
    ivec4(int x, int y, int z, int w) : data{x, y, z, w} {}
    constexpr
    ivec4(const ivec3& v, int w) : data{v.x, v.y, v.z, w} {}

    int *ptr() { return data; }
    const int *ptr() const { return data; }
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
