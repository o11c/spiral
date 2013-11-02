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
    constexpr float x() const { return data[0]; }
    constexpr float y() const { return data[1]; }

    constexpr float r() const { return data[0]; }
    constexpr float g() const { return data[1]; }

    constexpr float s() const { return data[0]; }
    constexpr float t() const { return data[1]; }

    void x(float v) { data[0] = v; }
    void y(float v) { data[1] = v; }

    void r(float v) { data[0] = v; }
    void g(float v) { data[1] = v; }

    void s(float v) { data[0] = v; }
    void t(float v) { data[1] = v; }
private:
    float data[2];
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
    constexpr float x() const { return data[0]; }
    constexpr float y() const { return data[1]; }
    constexpr float z() const { return data[2]; }

    constexpr float r() const { return data[0]; }
    constexpr float g() const { return data[1]; }
    constexpr float b() const { return data[2]; }

    constexpr float s() const { return data[0]; }
    constexpr float t() const { return data[1]; }
    constexpr float p() const { return data[2]; }

    void x(float v) { data[0] = v; }
    void y(float v) { data[1] = v; }
    void z(float v) { data[2] = v; }

    void r(float v) { data[0] = v; }
    void g(float v) { data[1] = v; }
    void b(float v) { data[2] = v; }

    void s(float v) { data[0] = v; }
    void t(float v) { data[1] = v; }
    void p(float v) { data[2] = v; }
private:
    float data[3];
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
    constexpr float x() const { return data[0]; }
    constexpr float y() const { return data[1]; }
    constexpr float z() const { return data[2]; }
    constexpr float w() const { return data[3]; }

    constexpr float r() const { return data[0]; }
    constexpr float g() const { return data[1]; }
    constexpr float b() const { return data[2]; }
    constexpr float a() const { return data[3]; }

    constexpr float s() const { return data[0]; }
    constexpr float t() const { return data[1]; }
    constexpr float p() const { return data[2]; }
    constexpr float q() const { return data[3]; }

    void x(float v) { data[0] = v; }
    void y(float v) { data[1] = v; }
    void z(float v) { data[2] = v; }
    void w(float v) { data[3] = v; }

    void r(float v) { data[0] = v; }
    void g(float v) { data[1] = v; }
    void b(float v) { data[2] = v; }
    void a(float v) { data[3] = v; }

    void s(float v) { data[0] = v; }
    void t(float v) { data[1] = v; }
    void p(float v) { data[2] = v; }
    void q(float v) { data[3] = v; }
private:
    float data[4];
public:
    vec4() = default;
    constexpr
    vec4(float x, float y, float z, float w) : data{x, y, z, w} {}
    constexpr
    vec4(const vec3& v, float w) : data{v.x(), v.y(), v.z(), w} {}

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

#endif // VECTOR_HPP
