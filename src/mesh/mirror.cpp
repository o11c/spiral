#include "mirror.hpp"

Mirror::Mirror(NewTextureProgram *ntp, YamlMesh surf, std::unique_ptr<Model> obj)
: surface(ntp, surf)
, surface_planes(surf.faces.size())
, reflectee(std::move(obj))
{
    size_t num_faces = surf.faces.size();
    for (size_t i = 0; i < num_faces; ++i)
    {
        ivec3 indices = surf.faces[i].vertices;
        vec3 pos[3] =
        {
            surf.vertices[indices.data[0]].position.xyz,
            surf.vertices[indices.data[1]].position.xyz,
            surf.vertices[indices.data[2]].position.xyz,
        };
        vec3 norm = cross(pos[1] - pos[0], pos[2] - pos[0]);
        surface_planes[i] = vec4(norm, dot(pos[0], norm));
        printf("%f = %f = %f\n", dot(pos[0], norm), dot(pos[1], norm), dot(pos[2], norm));
    }
}
void Mirror::draw(Context& c)
{
    surface.draw(c);
    reflectee->draw(c);
}
