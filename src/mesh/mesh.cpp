#include "mesh.hpp"

#include "../glue/gl_wrap.hpp"
#include "../math/vector.hpp"

Mesh::Mesh(TextureProgram *tp, YamlMesh ym)
: texture_program(tp)
{
    glGenBuffers(4, &mesh_points);

    // textures ignored for now
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

void Mesh::draw()
{
    texture_program->load();

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
