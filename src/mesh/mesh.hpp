#ifndef MESH_HPP
#define MESH_HPP

#include "../../config.h"

#include "scene.hpp"
#include "../yaml/dumb.hpp"
#include "../shaders/new-texture-program.hpp"

class Mesh : public Model
{
    NewTextureProgram *texture_program;
    unsigned int mesh_points, mesh_norms, mesh_texture_coords, mesh_indices;
    unsigned int face_count;
    std::unique_ptr<Material> material;
public:
    Mesh(NewTextureProgram *tp, YamlMesh ym);
    ~Mesh();

    void draw(Context&) override;
    void draw_sub(Context&, unsigned start, unsigned len);
};

#endif // MESH_HPP
