#ifndef QUAT_HPP
#define QUAT_HPP

class vec3;
class mat4;

class quat
{
public:
    union
    {
        float data[4];
        struct { float x, y, z, w; };
    };

    vec3 xyz() const;
    operator mat4() const;
public:
    quat() = default;
    quat(float angle, vec3 axis);

    void norm();
};

quat operator * (const quat& l, const quat& r);
quat& operator *= (quat& l, const quat& r);

#endif //QUAT_HPP
