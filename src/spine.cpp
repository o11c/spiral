#include "spine.hpp"

#include <cmath>

#include "gl_wrap.hpp"
#include "vector.hpp"

Spine::Spine(FlatProgram *fp, ShadeProgram *sp)
: flat_program(fp)
, shade_program(sp)
, spine(false)
, mesh_rings(false)
, mesh_longs(false)
, shade(false)
, dirty_spine(true)
, dirty_mesh(true)
{
    glGenBuffers(6, &spine_points);
}

Spine::~Spine()
{
    glDeleteBuffers(6, &spine_points);
}

vec3 Spine::C(float t)
{
    float x = (a + b * cos(q * t)) * cos(p * t);
    float y = (a + b * cos(q * t)) * sin(p * t);
    float z = b * sin(q * t);
    return {x, y, z};
}
vec3 Spine::T(float t)
{
    // in CSE we trust
    vec3 c = C(t);
    float dx = -p * c.y - b * q * sin(q * t) * cos(p * t);
    float dy = p * c.x - b * q * sin(q * t) * sin(p * t);
    float dz = b * q * cos(q * t);
    return {dx, dy, dz};
}

vec3 Spine::A(float t)
{
    vec3 d = T(t);
    float ax = -p * d.y + b * q * (p * sin(q * t) * sin(p * t) - q * cos(q * t) * cos(p * t));
    float ay = p * d.x - b * q * (p * sin(q * t) * cos(p * t) + q * cos(q * t) * sin(p * t));
    float az = -b * q * q * sin(q * t);
    return {ax, ay, az};
}

vec3 Spine::B(float t)
{
    return cross(T(t), A(t));
}

vec3 Spine::N(float t)
{
    return cross(B(t), T(t));
}


void Spine::draw()
{
    if (spine)
        draw_spine();
    if (mesh_rings or mesh_longs)
        draw_mesh();
    if (shade)
        draw_shade();
}

void Spine::update_spine()
{
    if (!dirty_spine)
        return;
    int N = n * p * q;
    dirty_spine = false;
    vec3 points[N];
    vec2 params[N];
    for (int i = 0; i < N; ++i)
    {
        float t = 2 * M_PI * i / N;
        params[i] = {t, 0};
        points[i] = C(t);
    }
    glBindBuffer(GL_ARRAY_BUFFER, spine_points);
    glBufferData(GL_ARRAY_BUFFER,
            sizeof(points), points,
            GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, spine_params);
    glBufferData(GL_ARRAY_BUFFER,
            sizeof(params), params,
            GL_STATIC_DRAW);
}

void Spine::draw_spine()
{
    update_spine();
    flat_program->load();

    int N = n * p * q;
    {
        glEnableVertexAttribArray(flat_program->vertexPositionAttribute);
        glEnableVertexAttribArray(flat_program->paramAttribute);
        glBindBuffer(GL_ARRAY_BUFFER, spine_points);
        glVertexAttribPointer(flat_program->vertexPositionAttribute, 3, GL_FLOAT, GL_FALSE,
                0, (GLvoid*) 0);
        glBindBuffer(GL_ARRAY_BUFFER, spine_params);
        glVertexAttribPointer(flat_program->paramAttribute, 2, GL_FLOAT, GL_FALSE,
                0, (GLvoid*) 0);
        glDrawArrays(GL_LINE_LOOP, 0, N);
        glDisableVertexAttribArray(flat_program->vertexPositionAttribute);
        glDisableVertexAttribArray(flat_program->paramAttribute);
    }
}

void Spine::update_mesh()
{
    if (!dirty_mesh)
        return;
    int N = n * p * q;
    int M = m;
    dirty_mesh = false;
    vec3 points[N * M];
    vec3 norms[N * M];
    vec2 params[N * M];
    struct sv2
    {
        unsigned short a, b;
    };
    sv2 quad_indices[N * (M + 1)];
    for (int i = 0; i < N; ++i)
    {
        float t = 2 * M_PI * i / N;
        vec3 Ct = C(t);
        vec3 Bt = B(t);
        norm3(Bt);
        vec3 Nt = this->N(t);
        norm3(Nt);
        for (int j = 0; j < M; ++j)
        {
            float u = 2 * M_PI * j / M;
            params[i * M + j] = {t, u};
            norms[i * M + j] = cos(u) * Bt + sin(u) * Nt;
            points[i * M + j] = Ct + r * norms[i * M + j];
            quad_indices[i * (M + 1) + j] = {
                (unsigned short)(i * M + j),
                (unsigned short)((i + 1) % N * M + j),
            };
        }
        quad_indices[i * (M + 1) + M] = quad_indices[i * (M + 1) + 0];
    }

    glBindBuffer(GL_ARRAY_BUFFER, mesh_points);
    glBufferData(GL_ARRAY_BUFFER,
            sizeof(points), points,
            GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, mesh_norms);
    glBufferData(GL_ARRAY_BUFFER,
            sizeof(norms), norms,
            GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, mesh_params);
    glBufferData(GL_ARRAY_BUFFER,
            sizeof(params), params,
            GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_indices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            sizeof(quad_indices), quad_indices,
            GL_STATIC_DRAW);
}

void Spine::draw_mesh()
{
    update_mesh();
    flat_program->load();

    int N = n * p * q;
    int M = m;

    glEnableVertexAttribArray(flat_program->vertexPositionAttribute);
    glEnableVertexAttribArray(flat_program->paramAttribute);
    if (mesh_rings)
    {
        // rings around the spine
        // there are N of these, and each has M points
        // and they are adjacent
        // with M=5, N=3
        // 11111
        //      22222
        //           33333
        glBindBuffer(GL_ARRAY_BUFFER, mesh_points);
        glVertexAttribPointer(flat_program->vertexPositionAttribute, 3, GL_FLOAT, GL_FALSE,
                0, (GLvoid*) 0);
        glBindBuffer(GL_ARRAY_BUFFER, mesh_params);
        glVertexAttribPointer(flat_program->paramAttribute, 2, GL_FLOAT, GL_FALSE,
                0, (GLvoid*) 0);
        for (int i = 0; i < N; ++i)
        {
            glDrawArrays(GL_LINE_LOOP, i * M, M);
        }
    }
    if (mesh_longs)
    {
        // the long bits "parallel" to the spine
        // there are M of these, and each has N points
        // but they are interleaved
        // with M=5, N=3
        // 1    1    1
        //  2    2    2
        //   3    3    3
        //    4    4    4
        //     5    5    5
        for (int j = 0; j < M; ++j)
        {
            glBindBuffer(GL_ARRAY_BUFFER, mesh_points);
            glVertexAttribPointer(flat_program->vertexPositionAttribute, 3, GL_FLOAT, GL_FALSE,
                    M * sizeof(vec3), (GLvoid*) (j * sizeof(vec3)));
            glBindBuffer(GL_ARRAY_BUFFER, mesh_params);
            glVertexAttribPointer(flat_program->paramAttribute, 2, GL_FLOAT, GL_FALSE,
                    M * sizeof(vec2), (GLvoid*) (j * sizeof(vec2)));
            glDrawArrays(GL_LINE_LOOP, 0, N);
        }
    }
    glDisableVertexAttribArray(flat_program->vertexPositionAttribute);
    glDisableVertexAttribArray(flat_program->paramAttribute);
}

void Spine::draw_shade()
{
    update_mesh();
    shade_program->load();

    int N = n * p * q;
    int M = m;

    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0f, 1.0f);
    glEnableVertexAttribArray(shade_program->vertexPositionAttribute);
    glEnableVertexAttribArray(shade_program->vertexNormalAttribute);
    glEnableVertexAttribArray(shade_program->paramAttribute);
    {
        glBindBuffer(GL_ARRAY_BUFFER, mesh_points);
        glVertexAttribPointer(shade_program->vertexPositionAttribute, 3, GL_FLOAT, GL_FALSE,
                0, (GLvoid*) 0);
        glBindBuffer(GL_ARRAY_BUFFER, mesh_norms);
        glVertexAttribPointer(shade_program->vertexNormalAttribute, 3, GL_FLOAT, GL_FALSE,
                0, (GLvoid*) 0);
        glBindBuffer(GL_ARRAY_BUFFER, mesh_params);
        glVertexAttribPointer(shade_program->paramAttribute, 2, GL_FLOAT, GL_FALSE,
                0, (GLvoid*) 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_indices);
        //for (int i = 0; i < N; ++i)
        {
            glDrawElements(GL_TRIANGLE_STRIP, 2 * (M + 1) * N, GL_UNSIGNED_SHORT,
                    (GLvoid *) 0);
        }
    }
    glDisableVertexAttribArray(shade_program->vertexPositionAttribute);
    glDisableVertexAttribArray(shade_program->vertexNormalAttribute);
    glDisableVertexAttribArray(shade_program->paramAttribute);
    glDisable(GL_POLYGON_OFFSET_FILL);
}
