#include "matrix.hpp"

#include <cmath>

#include "angle.hpp"

// Based on matrix.c by Wayne Cochran, as bundled with the glyphs project.

mat2::mat2() : data{
        1, 0,
        0, 1,
}
{}

// Column-Major indexing of 4x4 matrix
float& mat2::operator()(size_t r, size_t c)
{
    return data[c * 2 + r];
}
const float& mat2::operator()(size_t r, size_t c) const
{
    return data[c * 2 + r];
}

mat3::mat3() : data{
        1, 0, 0,
        0, 1, 0,
        0, 0, 1,
}
{}

// Column-Major indexing of 4x4 matrix
float& mat3::operator()(size_t r, size_t c)
{
    return data[c * 3 + r];
}
const float& mat3::operator()(size_t r, size_t c) const
{
    return data[c * 3 + r];
}

mat4::mat4() : data{
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1,
}
{}

// Column-Major indexing of 4x4 matrix
float& mat4::operator()(size_t r, size_t c)
{
    return data[c * 4 + r];
}
const float& mat4::operator()(size_t r, size_t c) const
{
    return data[c * 4 + r];
}
vec4 mat4::row(size_t r) const
{
    return
    {
        (*this)(r, 0),
        (*this)(r, 1),
        (*this)(r, 2),
        (*this)(r, 3),
    };
}
vec4 mat4::col(size_t c) const
{
    return
    {
        (*this)(0, c),
        (*this)(1, c),
        (*this)(2, c),
        (*this)(3, c),
    };
}

static
void matrixSet4x4(mat4& M,
                  float m00, float m01, float m02, float m03,
                  float m10, float m11, float m12, float m13,
                  float m20, float m21, float m22, float m23,
                  float m30, float m31, float m32, float m33)
{
    M(0, 0)=m00; M(0, 1)=m01; M(0, 2)=m02; M(0, 3)=m03;
    M(1, 0)=m10; M(1, 1)=m11; M(1, 2)=m12; M(1, 3)=m13;
    M(2, 0)=m20; M(2, 1)=m21; M(2, 2)=m22; M(2, 3)=m23;
    M(3, 0)=m30; M(3, 1)=m31; M(3, 2)=m32; M(3, 3)=m33;
}

mat4 operator * (const mat4& A, const mat4& B)
{
    mat4 AB;
    for (int r = 0; r < 4; r++)
        for (int c = 0; c < 4; c++)
        {
            float s = 0;
            for (int i = 0; i < 4; i++)
                s += A(r, i) * B(i, c);
            AB(r, c) = s;
        }
    return AB;
}

vec4 operator * (const mat4& M, const vec4& V)
{
    return
    {
        dot(M.row(0), V),
        dot(M.row(1), V),
        dot(M.row(2), V),
        dot(M.row(3), V),
    };
}

void mat4::operator *= (const mat4& A)
{
    *this = *this * A;
}

void mat4::lookat(const vec3& eye, const vec3& center, const vec3& up)
{
    vec3 F = center - eye;
    norm3(F);
    vec3 U = up;
    vec3 S = cross(F, U);
    norm3(S);
    U = cross(S, F);
    mat4 R;
    matrixSet4x4(R,
                S.x,  S.y,  S.z, 0,
                U.x,  U.y,  U.z, 0,
               -F.x, -F.y, -F.z, 0,
                0,    0,    0,   1);
    *this *= R;
    this->translate(-eye);
}

void mat4::ortho(
        float left, float right,
        float bottom, float top,
        float hither, float yon)
{
    mat4 S;
    matrixSet4x4(S,
                 2/(right - left), 0,  0, -(right + left)/(right - left),
                 0, 2/(top - bottom), 0, -(top + bottom)/(top - bottom),
                 0, 0, -2/(yon-hither), -(yon + hither)/(yon - hither),
                 0, 0, 0,               1);
    *this *= S;
}

void mat4::perspective(
        Degrees fovy_degrees,
        float aspect,
        float zNear, float zFar)
{
    const Radians fovy = fovy_degrees;
    const float f = 1/tan_(fovy/2);
    const float s = 1/(zNear - zFar);
    mat4 P;
    matrixSet4x4(P,
                 f/aspect, 0, 0, 0,
                 0, f, 0, 0,
                 0, 0, (zFar + zNear)*s, 2*zFar*zNear*s,
                 0, 0, -1, 0);
    *this *= P;
}

void mat4::scale(const vec3& s)
{
    mat4 S;
    matrixSet4x4(S,
                 s.x, 0,  0,  0,
                 0,  s.y, 0,  0,
                 0,  0,  s.z, 0,
                 0,  0,  0,  1);
    *this *= S;
}

void mat4::translate(const vec3& d)
{
    mat4 S;
    matrixSet4x4(S,
                 1, 0, 0, d.x,
                 0, 1, 0, d.y,
                 0, 0, 1, d.z,
                 0, 0, 0, 1);
    *this *= S;
}

void mat4::rotate(Degrees angle_degrees, vec3 axis)
{
    norm3(axis);
    const float x = axis.x;
    const float y = axis.y;
    const float z = axis.z;

    const Radians angle = angle_degrees;
    const float c = cos_(angle);
    const float s = sin_(angle);
    const float c_ = 1 - c;
    const float zc_ = z * c_;
    const float yc_ = y * c_;
    const float xzc_ = x * zc_;
    const float xyc_ = x * y * c_;
    const float yzc_ = y * zc_;
    const float xxc_ = x * x * c_;
    const float yyc_ = y * yc_;
    const float zzc_ = z * zc_;
    const float xs = x * s;
    const float ys = y * s;
    const float zs = z * s;
    mat4 S;
    matrixSet4x4(S,
                xxc_ + c,   xyc_ - zs,  xzc_ + ys,  0,
                xyc_ + zs,  yyc_ + c,   yzc_ - xs,  0,
                xzc_ - ys,  yzc_ + xs,  zzc_ + c,   0,
                0,          0,          0,          1);
    *this *= S;
}

//
// Computes inverse-transpose of upper 3x3 of M.
//
mat3 mat4::get_normal() const
{
    const mat4& M = *this;
    const double determinant =
      +M(0, 0) * (M(1, 1) * M(2, 2) - M(2, 1) * M(1, 2))
      -M(0, 1) * (M(1, 0) * M(2, 2) - M(1, 2) * M(2, 0))
      +M(0, 2) * (M(1, 0) * M(2, 1) - M(1, 1) * M(2, 0));
    const double invDet = 1.0/determinant;


    mat3 N;

    N(0, 0) = +(M(1, 1) * M(2, 2) - M(2, 1) * M(1, 2)) * invDet;
    N(1, 0) = -(M(0, 1) * M(2, 2) - M(0, 2) * M(2, 1)) * invDet;
    N(2, 0) = +(M(0, 1) * M(1, 2) - M(0, 2) * M(1, 1)) * invDet;

    N(0, 1) = -(M(1, 0) * M(2, 2) - M(1, 2) * M(2, 0)) * invDet;
    N(1, 1) = +(M(0, 0) * M(2, 2) - M(0, 2) * M(2, 0)) * invDet;
    N(2, 1) = -(M(0, 0) * M(1, 2) - M(1, 0) * M(0, 2)) * invDet;

    N(0, 2) = +(M(1, 0) * M(2, 1) - M(2, 0) * M(1, 1)) * invDet;
    N(1, 2) = -(M(0, 0) * M(2, 1) - M(2, 0) * M(0, 1)) * invDet;
    N(2, 2) = +(M(0, 0) * M(1, 1) - M(1, 0) * M(0, 1)) * invDet;

    return N;
}

void mat4::reflect(const vec4& plane)
{
    mat4 R;
    const float A = plane.x;
    const float B = plane.y;
    const float C = plane.z;
    const float D = plane.w;
    matrixSet4x4(R,
                 1 -2*A*A,  -2*A*B,  -2*A*C, -2*A*D,
                   -2*B*A, 1-2*B*B,  -2*B*C, -2*B*D,
                   -2*C*A,  -2*C*B, 1-2*C*C, -2*C*D,
                 0,        0,       0,       1);
    *this *= R;
}

void mat4::shadow(const vec4& light, const vec4& plane)
{
    // plane's xyzw is really ABCD
    mat4 S;

    float dot = ::dot(light, plane);

    S(0, 0) = dot - light.x * plane.x;
    S(0, 1) =     - light.x * plane.y;
    S(0, 2) =     - light.x * plane.z;
    S(0, 3) =     - light.x * plane.w;

    S(1, 0) =     - light.y * plane.x;
    S(1, 1) = dot - light.y * plane.y;
    S(1, 2) =     - light.y * plane.z;
    S(1, 3) =     - light.y * plane.w;

    S(2, 0) =     - light.z * plane.x;
    S(2, 1) =     - light.z * plane.y;
    S(2, 2) = dot - light.z * plane.z;
    S(2, 3) =     - light.z * plane.w;

    S(3, 0) =     - light.w * plane.x;
    S(3, 1) =     - light.w * plane.y;
    S(3, 2) =     - light.w * plane.z;
    S(3, 3) = dot - light.w * plane.w;

    *this *= S;
}
