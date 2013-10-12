#ifndef SPINE_HPP
#define SPINE_HPP

#include "drawing.hpp"
#include "flat-program.hpp"

class Spine : public Drawing
{
    FlatProgram *flat_program;
public:
    float a, b, r;
    int p, q;
    int n, m;
    bool spine;
    bool mesh_rings, mesh_longs;

    bool dirty_spine, dirty_mesh;
private:
    unsigned int spine_points, spine_params;
    unsigned int mesh_points, mesh_params;
public:
    Spine(FlatProgram *fp);
    ~Spine();

    vec3 C(float t);
    vec3 T(float t);
    vec3 A(float t);
    vec3 B(float t);
    vec3 N(float t);

    void draw() override;
private:
    void draw_spine();
    void draw_mesh();
};

#endif
