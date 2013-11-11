#ifndef ANGLE_HPP
#define ANGLE_HPP

#include <cmath>

class Turns;
class Radians;
class Degrees;

class Turns
{
    float _value;
public:
    Turns() = default;
    constexpr explicit
    Turns(float n, float d=1.0f) : _value(n/d) {}
    constexpr
    Turns(Radians d);
    constexpr
    Turns(Degrees d);

    constexpr
    float value() { return _value; }
};

class Radians
{
    float _value;
public:
    Radians() = default;
    constexpr explicit
    Radians(float v) : _value(v) {}
    constexpr
    Radians(Turns d);
    constexpr
    Radians(Degrees d);

    constexpr
    float value() { return _value; }
};

class Degrees
{
    float _value;
public:
    Degrees() = default;
    constexpr explicit
    Degrees(float v) : _value(v) {}
    constexpr
    Degrees(Turns t);
    constexpr
    Degrees(Radians r);

    constexpr
    float value() { return _value; }
};

constexpr
Turns::Turns(Radians r)
: _value(r.value() / (2.0f * M_PI))
{}
constexpr
Turns::Turns(Degrees d)
: _value(d.value() / 360.0f)
{}

constexpr
Radians::Radians(Turns t)
: _value(t.value() * 2.0f * M_PI)
{}
constexpr
Radians::Radians(Degrees d)
: _value(d.value() / 180.0f * M_PI)
{}

constexpr
Degrees::Degrees(Turns t)
: _value(t.value() * 360.0f)
{}
constexpr
Degrees::Degrees(Radians r)
: _value(r.value() * 180.0f / M_PI)
{}

inline
float sin_(Radians r)
{
    return sinf(r.value());
}

inline
float cos_(Radians r)
{
    return cosf(r.value());
}

inline
float tan_(Radians r)
{
    return tanf(r.value());
}

inline bool operator == (Radians l, Radians r)
{ return l.value() == r.value(); }
inline bool operator != (Radians l, Radians r)
{ return l.value() != r.value(); }
inline bool operator < (Radians l, Radians r)
{ return l.value() < r.value(); }
inline bool operator > (Radians l, Radians r)
{ return l.value() > r.value(); }
inline bool operator <= (Radians l, Radians r)
{ return l.value() <= r.value(); }
inline bool operator >= (Radians l, Radians r)
{ return l.value() >= r.value(); }

inline Radians operator + (Radians l, Radians r)
{ return Radians(l.value() + r.value()); }
inline Radians operator - (Radians l, Radians r)
{ return Radians(l.value() - r.value()); }
inline Radians operator * (Radians l, float r)
{ return Radians(l.value() * r); }
inline Radians operator * (float l, Radians r)
{ return Radians(l * r.value()); }
inline Radians operator / (Radians l, float r)
{ return Radians(l.value() / r); }

inline Radians& operator += (Radians& l, Radians r)
{ return l = l + r; }
inline Radians& operator -= (Radians& l, Radians r)
{ return l = l - r; }
inline Radians& operator *= (Radians& l, float r)
{ return l = l * r; }
inline Radians& operator /= (Radians& l, float r)
{ return l = l / r; }

#endif //ANGLE_HPP
