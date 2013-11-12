#include "spine.hpp"

#include <cmath>
#include <cstdio>

#include "../glue/gl_wrap.hpp"
#include "../math/vector.hpp"

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

vec3 Spine::C(Radians t)
{
    float x = (a + b * cos_(q * t)) * cos_(p * t);
    float y = (a + b * cos_(q * t)) * sin_(p * t);
    float z = b * sin_(q * t);
    return {x, y, z};
}
vec3 Spine::T(Radians t)
{
    // in CSE we trust
    vec3 c = C(t);
    float dx = -p * c.y - b * q * sin_(q * t) * cos_(p * t);
    float dy = p * c.x - b * q * sin_(q * t) * sin_(p * t);
    float dz = b * q * cos_(q * t);
    return {dx, dy, dz};
}

vec3 Spine::A(Radians t)
{
    vec3 d = T(t);
    float ax = -p * d.y + b * q * (p * sin_(q * t) * sin_(p * t) - q * cos_(q * t) * cos_(p * t));
    float ay = p * d.x - b * q * (p * sin_(q * t) * cos_(p * t) + q * cos_(q * t) * sin_(p * t));
    float az = -b * q * q * sin_(q * t);
    return {ax, ay, az};
}

vec3 Spine::B(Radians t)
{
    return cross(T(t), A(t));
}

vec3 Spine::N(Radians t)
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
    vec4 params[N];
    for (int i = 0; i < N; ++i)
    {
        Turns t_(i, N);
        Radians t = t_;
        params[i] = {t_.value(), 0, 0, 1};
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
        glVertexAttribPointer(flat_program->paramAttribute, 4, GL_FLOAT, GL_FALSE,
                0, (GLvoid*) 0);
        glDrawArrays(GL_LINE_STRIP, 0, N+1);
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
    while ((N + 1) * (M + 1) >= 65536)
        N--;
    dirty_mesh = false;
    vec3 points[(N + 1) * (M + 1)];
    vec3 norms[(N + 1) * (M + 1)];
    vec4 params[(N + 1) * (M + 1)];
    struct sv2
    {
        unsigned short a, b;
    };
    sv2 quad_indices[N * (M + 1)];
    for (int i = 0; i <= N; ++i)
    {
        Turns t_(i, N);
        Radians t = t_;
        vec3 Ct = C(t);
        vec3 Bt = B(t);
        norm3(Bt);
        vec3 Nt = this->N(t);
        norm3(Nt);
        for (int j = 0; j <= M; ++j)
        {
            Turns u_(j, M);
            Radians u = u_;
            params[i * (M + 1) + j] = {t_.value() * q, u_.value(), 0, 1};
            norms[i * (M + 1) + j] = s * cos_(u) * Bt + r * sin_(u) * Nt;
            norm3(norms[i * (M + 1) + j]);
            points[i * (M + 1) + j] = Ct + r * cos_(u) * Bt + s * sin_(u) * Nt;
            if (i == N)
                continue;
            quad_indices[i * (M + 1) + j] = {
                (unsigned short)(i * (M + 1) + j),
                (unsigned short)((i + 1) * (M + 1) + j),
            };
        }
    }
    char filename[256];
    sprintf(filename, "data/spiral-%.10e,%.10e,%.10e,%.10e-%d,%d,%d,%d.mesh",
            a, b, r, s,
            p, q, N, M);
    FILE *fp = fopen(filename, "w");
    fprintf(fp, "textures:\n");
    fprintf(fp, "  ambient: data/earthlights1k.bmp\n");
    fprintf(fp, "  diffuse: data/earthmap1k.bmp\n");
    fprintf(fp, "  specular: data/earthspec1k.bmp\n");

    fprintf(fp, "\nvertices:\n");
    for (int i = 0; i < (N + 1) * (M + 1); ++i)
    {
        auto p = vec4(points[i], 1.0f);
        auto t = params[i];
        auto n = vec4(norms[i], 1.0f);
        fprintf(fp, "  - position: [%.10e, %.10e, %.10e, %.10e]\n", p.x, p.y, p.z, p.w);
        fprintf(fp, "    texture: [%.10e, %.10e, %.10e, %.10e]\n", t.x * this->q, t.y, t.z, t.w);
        fprintf(fp, "    normal: [%.10e, %.10e, %.10e, %.10e]\n", n.x, n.y, n.z, n.w);
    }

    fprintf(fp, "\nfaces:\n");
    for (int j = 1; j < N * (M + 1); ++j)
    {
        ivec3 f1 =
        {
            quad_indices[j - 1].a,
            quad_indices[j - 1].b,
            quad_indices[j].a,
        };
        ivec3 f2 =
        {
            quad_indices[j - 1].b,
            quad_indices[j].b,
            quad_indices[j].a,
        };
        fprintf(fp, "  - [%d, %d, %d]\n", f1.x, f1.y, f1.z);
        fprintf(fp, "  - [%d, %d, %d]\n", f2.x, f2.y, f2.z);
    }
    fclose(fp);

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
    while ((N + 1) * (M + 1) >= 65536)
        N--;

    glEnableVertexAttribArray(flat_program->vertexPositionAttribute);
    glEnableVertexAttribArray(flat_program->paramAttribute);
    if (mesh_rings)
    {
        // rings around the spine
        // there are N(+1) of these, and each has M(+1) points
        // and they are adjacent
        // with M=5, N=3
        // 11111X
        //       22222X
        //             33333X
        //                   YYYYYX
        glBindBuffer(GL_ARRAY_BUFFER, mesh_points);
        glVertexAttribPointer(flat_program->vertexPositionAttribute, 3, GL_FLOAT, GL_FALSE,
                0, (GLvoid*) 0);
        glBindBuffer(GL_ARRAY_BUFFER, mesh_params);
        glVertexAttribPointer(flat_program->paramAttribute, 4, GL_FLOAT, GL_FALSE,
                0, (GLvoid*) 0);
        for (int i = 0; i < N; ++i)
        {
            glDrawArrays(GL_LINE_STRIP, i * (M + 1), M + 1);
        }
    }
    if (mesh_longs)
    {
        // the long bits "parallel" to the spine
        // there are M(+1) of these, and each has N(+1) points
        // but they are interleaved
        // with M=5, N=3
        // 1     1     1     Y
        //  2     2     2     Y
        //   3     3     3     Y
        //    4     4     4     Y
        //     5     5     5     Y
        //      X     X     X     X
        for (int j = 0; j < M; ++j)
        {
            glBindBuffer(GL_ARRAY_BUFFER, mesh_points);
            glVertexAttribPointer(flat_program->vertexPositionAttribute, 3, GL_FLOAT, GL_FALSE,
                    (M + 1) * sizeof(vec3), (GLvoid*) (j * sizeof(vec3)));
            glBindBuffer(GL_ARRAY_BUFFER, mesh_params);
            glVertexAttribPointer(flat_program->paramAttribute, 4, GL_FLOAT, GL_FALSE,
                    (M + 1) * sizeof(vec4), (GLvoid*) (j * sizeof(vec4)));
            glDrawArrays(GL_LINE_STRIP, 0, N + 1);
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
    while ((N + 1) * (M + 1) >= 65536)
        N--;

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
        glVertexAttribPointer(shade_program->paramAttribute, 4, GL_FLOAT, GL_FALSE,
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
