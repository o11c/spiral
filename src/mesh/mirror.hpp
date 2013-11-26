#ifndef MESH_MIRROR_HPP
#define MESH_MIRROR_HPP

#include "mesh.hpp"
#include "scene.hpp"

class Mirror : public Model
{
    Mesh surface;
    std::vector<vec4> surface_planes;
    std::unique_ptr<Model> reflectee;
public:
    Mirror(NewTextureProgram *ntp, YamlMesh surf, std::unique_ptr<Model> obj);
    void draw(Context&) override;
};

#endif //MESH_MIRROR_HPP
