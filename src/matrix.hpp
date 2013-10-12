#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "vector.hpp"

class mat2
{
    float data[2*2];
public:
    // construct an identity matrix
    mat2();
    // indexing
    float& operator()(size_t r, size_t c);
    const float& operator()(size_t r, size_t c) const;

    float *ptr() { return data; }
    const float *ptr() const { return data; }
};

class mat3
{
    float data[3*3];
public:
    // construct an identity matrix
    mat3();
    // indexing
    float& operator()(size_t r, size_t c);
    const float& operator()(size_t r, size_t c) const;

    float *ptr() { return data; }
    const float *ptr() const { return data; }
};

class mat4
{
    float data[4*4];
public:
    // construct an identity matrix
    mat4();
    // indexing
    float& operator()(size_t r, size_t c);
    const float& operator()(size_t r, size_t c) const;

    float *ptr() { return data; }
    const float *ptr() const { return data; }

    void operator *= (const mat4& A);
    void scale(const vec3& s);
    void translate(const vec3& d);
    void rotate(float angle_degrees, vec3 axis);

    mat3 get_normal() const;

    void lookat(const vec3& eye, const vec3& center, const vec3& up);

    void ortho(
            float left, float right,
            float bottom, float top,
            float hither, float yon);
    void perspective(
            float fovy_degrees, float aspect,
            float zNear, float zFar);

    void reflect(const vec4& plane);
    void shadow(const vec4& light, const vec4& plane);
};

mat4 operator * (const mat4& A, const mat4& B);

class SavingMatrix
{
    mat4 val;
    mat4& ref;
public:
    SavingMatrix(mat4& m) : val(m), ref(m)
    {}
    ~SavingMatrix()
    {
        ref = val;
    }
};

#endif // MATRIX_HPP
