#include "mesh.hpp"

#include "../bmp.hpp"
#include "../glue/gl_wrap.hpp"
#include "../lieu/mem.hpp"
#include "../math/vector.hpp"

Mesh::Mesh(NewTextureProgram *tp, YamlMesh ym)
: texture_program(tp)
, material(make_unique<Material>())
{
    // TODO see if I can get rid of the numbers (auto-generate in Program load?)
    material->ambient = make_unique<sampler2D>(0, Bmp(ym.textures["ambient"].filename.c_str()));
    material->diffuse = make_unique<sampler2D>(1, Bmp(ym.textures["diffuse"].filename.c_str()));
    material->specular = make_unique<sampler2D>(2, Bmp(ym.textures["specular"].filename.c_str()));
    //material->normal = make_unique<sampler2D>(3, Bmp(ym.textures["normal"].filename.c_str()));
    material->shininess = 100;

    glGenBuffers(4, &mesh_points);

    size_t N = ym.vertices.size();
    size_t M = ym.faces.size();
    vec3 poses[N];
    vec4 texes[N];
    vec3 norms[N];
    uint16_t indices[3 * M];
    face_count = M;
    for (size_t i = 0; i < N; ++i)
    {
        poses[i] = ym.vertices[i].position.xyz;
        texes[i] = ym.vertices[i].texture;
        norms[i] = ym.vertices[i].normal.xyz;
    }
    for (size_t j = 0; j < M; ++j)
    {
        indices[3 * j + 0] = ym.faces[j].vertices.data[0];
        indices[3 * j + 1] = ym.faces[j].vertices.data[1];
        indices[3 * j + 2] = ym.faces[j].vertices.data[2];
    }
    glBindBuffer(GL_ARRAY_BUFFER, mesh_points);
    glBufferData(GL_ARRAY_BUFFER,
            sizeof(poses), poses,
            GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, mesh_texture_coords);
    glBufferData(GL_ARRAY_BUFFER,
            sizeof(texes), texes,
            GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, mesh_norms);
    glBufferData(GL_ARRAY_BUFFER,
            sizeof(norms), norms,
            GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_indices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            sizeof(indices), indices,
            GL_STATIC_DRAW);
}

Mesh::~Mesh()
{
    glDeleteBuffers(4, &mesh_points);
}

void Mesh::draw(Context& ctx)
{
    texture_program->load(ctx, *material);

    glEnableVertexAttribArray(texture_program->vertexPositionAttribute);
    glEnableVertexAttribArray(texture_program->vertexNormalAttribute);
    glEnableVertexAttribArray(texture_program->paramAttribute);
    {
        glBindBuffer(GL_ARRAY_BUFFER, mesh_points);
        glVertexAttribPointer(texture_program->vertexPositionAttribute, 3, GL_FLOAT, GL_FALSE,
                0, (GLvoid*) 0);
        glBindBuffer(GL_ARRAY_BUFFER, mesh_texture_coords);
        glVertexAttribPointer(texture_program->paramAttribute, 4, GL_FLOAT, GL_FALSE,
                0, (GLvoid*) 0);
        glBindBuffer(GL_ARRAY_BUFFER, mesh_norms);
        glVertexAttribPointer(texture_program->vertexNormalAttribute, 3, GL_FLOAT, GL_FALSE,
                0, (GLvoid*) 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_indices);
        glDrawElements(GL_TRIANGLES, 3 * face_count, GL_UNSIGNED_SHORT, (GLvoid *) 0);
    }
    glDisableVertexAttribArray(texture_program->vertexPositionAttribute);
    glDisableVertexAttribArray(texture_program->vertexNormalAttribute);
    glDisableVertexAttribArray(texture_program->paramAttribute);
}
