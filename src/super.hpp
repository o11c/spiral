#ifndef SPINE_HPP
#define SPINE_HPP

#include "../config.h"

#include "drawing.hpp"
#include "flat-program.hpp"
#include "shade-program.hpp"

class Super : public Drawing
{
    FlatProgram *flat_program;
    ShadeProgram *shade_program;
public:
    float a, b, r, s;
    int p, q;
    int n, m;
    bool mesh_rings, mesh_longs;
    bool shade;

    bool dirty_mesh;
private:
    unsigned int mesh_points, mesh_norms, mesh_params, mesh_indices;
public:
    Super(FlatProgram *fp, ShadeProgram *sp);
    ~Super();

    vec3 C(float t);
    vec3 T(float t);
    vec3 A(float t);
    vec3 B(float t);
    vec3 N(float t);

    void draw() override;
private:
    void update_mesh();

    void draw_mesh();
    void draw_shade();
};

#endif
