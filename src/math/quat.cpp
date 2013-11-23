#include "quat.hpp"

#include <cmath>

#include "vector.hpp"
#include "matrix.hpp"

quat::quat(Radians angle, vec3 axis)
{
    Radians angle2 = angle / 2;
    float s = sin_(angle2);
    x = axis.x * s;
    y = axis.y * s;
    z = axis.z * s;
    w = cos_(angle2);
}

quat::operator mat4() const
{
    mat4 rv;

    float wx2 = w * x * 2;
    float wy2 = w * y * 2;
    float wz2 = w * z * 2;
    float xx2 = x * x * 2;
    float xy2 = x * y * 2;
    float xz2 = x * z * 2;
    float yy2 = y * y * 2;
    float yz2 = y * z * 2;
    float zz2 = z * z * 2;

    rv(0, 0) = 1 - yy2 - zz2;
    rv(0, 1) = xy2 - wz2;
    rv(0, 2) = xz2 + wy2;
    rv(1, 0) = xy2 + wz2;
    rv(1, 1) = 1 - xx2 - zz2;
    rv(1, 2) = yz2 - wx2;
    rv(2, 0) = xz2 - wy2;
    rv(2, 1) = yz2 + wx2;
    rv(2, 2) = 1 - xx2 - yy2;
    return rv;
}

vec3 quat::xyz() const
{
    return {x, y, z};
}

quat operator * (const quat& a, const quat& b)
{
    quat rv;
    rv.x = a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y;
    rv.y = a.w * b.y + a.y * b.w + a.z * b.x - a.x * b.z;
    rv.z = a.w * b.z + a.z * b.w + a.x * b.y - a.y * b.x;
    rv.w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z;
    return rv;
}

quat& operator *= (quat& l, const quat& r)
{
    l = l * r;
    return l;
}

void quat::norm()
{
    float s = x * x + y * y + z * z + w * w;
    if (s != 1.0f)
    {
        s = 1 / sqrtf(s);
        x *= s;
        y *= s;
        z *= s;
        w *= s;
    }
}

quat operator - (const quat& r)
{
    quat out;
    out.x = -r.x;
    out.y = -r.y;
    out.z = -r.z;
    out.w = r.w;
    return out;
}
