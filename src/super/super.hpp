#ifndef SUPER_HPP
#define SUPER_HPP

#include "../../config.h"

#include "../drawing.hpp"
#include "../shaders/flat-program.hpp"
#include "../shaders/shade-program.hpp"
#include "../shaders/texture-program.hpp"

class Super : public Drawing
{
    FlatProgram *flat_program;
    ShadeProgram *shade_program;
    TextureProgram *texture_program;
public:
    float d; bool tor; bool a;
    float em, en;
    int dm, dn;

    bool mesh_rings, mesh_longs;
    bool shade, texture;

    bool dirty_mesh;
private:
    unsigned int mesh_points, mesh_norms, mesh_params, mesh_indices;
public:
    Super(FlatProgram *fp, ShadeProgram *sp, TextureProgram *tp);
    ~Super();

    vec3 SE(Radians u, Radians v);
    vec3 ST(Radians u, Radians v);
    vec3 SN(Radians u, Radians v);

    void draw() override;
private:
    void update_mesh();

    void draw_mesh();
    void draw_shade();
};

#endif //SUPER_HPP
