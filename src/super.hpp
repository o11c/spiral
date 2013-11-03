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
    float d; bool tor;
    float em, en;
    int dm, dn;

    bool mesh_rings, mesh_longs;
    bool shade;

    bool dirty_mesh;
private:
    unsigned int mesh_points, mesh_norms, mesh_params, mesh_indices;
public:
    Super(FlatProgram *fp, ShadeProgram *sp);
    ~Super();

    vec3 SE(float u, float v);
    vec3 ST(float u, float v);
    vec3 SN(float u, float v);

    void draw() override;
private:
    void update_mesh();

    void draw_mesh();
    void draw_shade();
};

#endif
