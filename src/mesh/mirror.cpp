#include "mirror.hpp"

#include "../glue/gl_wrap.hpp"

Mirror::Mirror(NewTextureProgram *ntp, YamlMesh surf, float r, std::unique_ptr<Model> obj)
: surface(ntp, surf)
, ropac(r)
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
        norm3(norm);
        surface_planes[i] = vec4(norm, -dot(pos[0], norm));
    }
}

// It is probably a really bad idea to use a Mirror as a sub-object
void Mirror::draw(Context& c)
{
    glDepthRange(0.5f, 1.0f);
    reflectee->draw(c);

    glEnable(GL_STENCIL_TEST);
    glStencilMask(0x01);
    glBlendColor(0, 0, 0, ropac);
    glBlendFunc(GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA);

    for (size_t i = 0; i < surface_planes.size(); ++i)
    {
        glClear(GL_STENCIL_BUFFER_BIT);

        glStencilFunc(GL_ALWAYS, 0x01, 0);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        surface.draw_sub(c, i, 1);
        glStencilFunc(GL_EQUAL, 0x01, 0x01);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

        {
            // TODO figure out what's supposed to be done with the light

            glDepthRange(0.0f, 0.5f);
            //glClear(GL_DEPTH_BUFFER_BIT);
            SavingMatrix m(c.ModelView);
            c.ModelView.reflect(surface_planes[i]);
            //c.ModelView.scale({1, 1, -1});
            glFrontFace(GL_CW);
            reflectee->draw(c);
            glFrontFace(GL_CCW);
            glDepthRange(0.5f, 1.0f);
        }

        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        surface.draw_sub(c, i, 1);
        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
    }
    glDisable(GL_STENCIL_TEST);
    glDepthRange(0.0f, 1.0f);
    glStencilMask(0xff);
}
