#ifndef MESH_HPP
#define MESH_HPP

#include "../../config.h"

#include "../drawing.hpp"
#include "../yaml/dumb.hpp"
#include "../shaders/texture-program.hpp"

class Mesh : public Drawing
{
    TextureProgram *texture_program;
    unsigned int mesh_points, mesh_norms, mesh_texture_coords, mesh_indices;
    unsigned int face_count;
public:
    Mesh(TextureProgram *tp, YamlMesh ym);
    ~Mesh();

    void draw() override;
};

#endif // MESH_HPP
