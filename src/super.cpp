#include "super.hpp"

#include <cmath>
#include <cstdio>

#include "gl_wrap.hpp"
#include "vector.hpp"

Super::Super(FlatProgram *fp, ShadeProgram *sp)
: flat_program(fp)
, shade_program(sp)
, mesh_rings(false)
, mesh_longs(false)
, shade(false)
, dirty_mesh(true)
{
    glGenBuffers(4, &mesh_points);
}

Super::~Super()
{
    glDeleteBuffers(4, &mesh_points);
}

static
float safe_pow(float base, float expon)
{
    if (base < 0.0f)
        return -safe_pow(-base, expon);
    return pow(base, expon);
}

vec3 Super::SE(float u, float v)
{
    float x = safe_pow(cos(v), 2/em) * safe_pow(cos(u), 2/en);
    float y = safe_pow(cos(v), 2/em) * safe_pow(sin(u), 2/en);
    float z = safe_pow(sin(v), 2/em);
    return {x, y, z};
}

vec3 Super::ST(float u, float v)
{
    float x = (d + safe_pow(cos(v), 2/em)) * safe_pow(cos(u), 2/en);
    float y = (d + safe_pow(cos(v), 2/em)) * safe_pow(sin(u), 2/en);
    float z = safe_pow(sin(v), 2/em);
    return {x, y, z};
}

vec3 Super::SN(float u, float v)
{
    float x = safe_pow(cos(v), 2 - 2/em) * safe_pow(cos(u), 2 - 2/en);
    float y = safe_pow(cos(v), 2 - 2/em) * safe_pow(sin(u), 2 - 2/en);
    float z = safe_pow(sin(v), 2 - 2/em);
    return {x, y, z};
}

void Super::draw()
{
    if (mesh_rings or mesh_longs)
        draw_mesh();
    if (shade)
        draw_shade();
}

void Super::update_mesh()
{
    if (!dirty_mesh)
        return;
    int N = dn;
    int M = dm;
    dirty_mesh = false;
#define INDEX(i, j) ((i) * (M + 1) + (j))
    vec3 points[(N + 1) * (M + 1)];
    vec3 norms[(N + 1) * (M + 1)];
    vec4 params[(N + 1) * (M + 1)];
    struct sv2
    {
        unsigned short a, b;
    };
    sv2 quad_indices[N * ((M + 1) + 2) - 2];
    for (int i = 0; i <= N; ++i)
    {
        float t = float(i) / N;
        float v = (2.0f * t - 1.0f);
        if (tor)
            v *= M_PI;
        else
            v *= M_PI_2;

        if (i != 0 && i != N)
        {
            quad_indices[(i * (M + 3) - 1)] = {
                (unsigned short)(INDEX(i + 1, 0)),
                (unsigned short)(INDEX(i + 1, 0)),
            };
        }
        for (int j = 0; j <= M; ++j)
        {
            float s = float(j) / M;
            float u = (2.0f * s - 1.0f);
            u *= M_PI;
            points[INDEX(i, j)] = tor ? ST(u, v) : SE(u, v);
            norms[INDEX(i, j)] = SN(u, v);
            norm3(norms[INDEX(i, j)]);
            if (i == N)
                continue;
            quad_indices[(i * (M + 3) + j)] = {
                (unsigned short)(INDEX(i + 1, j)),
                (unsigned short)(INDEX(i, j)),
            };
        }
        if (i != N)
            quad_indices[(i * (M + 3) + (M + 1))] = {
                (unsigned short)(INDEX(i, M)),
                (unsigned short)(INDEX(i, M)),
            };
    }
    for (int i = 0; i <= N; ++i)
    {
        int si = i;
        if (i == 0)
            si = 1;
        if (i == N)
            si = N - 1;
        for (int j = 0; j <= M; ++j)
        {
            int tj = j;
            if (j == M)
                tj = 0;
            // giving up on this for now
            float s = float(j) / M;
            float t = float(i) / N;
            printf("(%f, %f) -> ", s, t);

            s = 0.0f;
            {
                float sn = 0.0f;
                float d = 0.0f;
                for (int k = 0; k < j; ++k)
                {
                    d = mag(points[INDEX(si, k + 1)] - points[INDEX(si, k)]);
                    sn += d;
                }
                float sd = sn;
                for (int k = j; k < M; ++k)
                {
                    float d = mag(points[INDEX(si, k + 1)] - points[INDEX(si, k)]);
                    sd += d;
                }
                printf("{+%f = %f/%f,", d, sn, sd);
                s = sn / sd;
            }

            t = 0.0f;
            {
                float tn = 0.0f;
                float d = 0.0f;
                for (int k = 0; k < i; ++k)
                {
                    d = mag(points[INDEX(k, tj)] - points[INDEX(k + 1, tj)]);
                    tn += d;
                }
                float td = tn;
                for (int k = i; k < N; ++k)
                {
                    float d = mag(points[INDEX(k, tj)] - points[INDEX(k + 1, tj)]);
                    td += d;
                }
                printf("+%f = %f/%f} = ", d, tn, td);
                t = tn / td;
            }

            printf("(%f, %f)", s, t);

            // q is necessary to make textures appear properly when the
            // faces are not squares; especially noticable at the poles.
            float q = 0.0;
            q += mag(points[INDEX(i, j)] - points[INDEX(i, (j + 1) % M)]);
            q += mag(points[INDEX(i, j)] - points[INDEX(i, (j - 1 + M) % M)]);
            printf("; %f\n", q);
            // but this seems to cause other issues
            if (a)
                q = 1;

            params[INDEX(i, j)] = {s * q, t * q, 0, 1 * q};
        }
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

void Super::draw_mesh()
{
    update_mesh();
    flat_program->load();

    int N = dn;
    int M = dm;

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
        for (int i = 1; i < N; ++i)
        {
            glDrawArrays(GL_LINE_STRIP, INDEX(i, 0), M + 1);
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

void Super::draw_shade()
{
    update_mesh();
    shade_program->load();

    int N = dn;
    int M = dm;

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
            glDrawElements(GL_TRIANGLE_STRIP, N * (2 * (M + 1) + 4) - 4, GL_UNSIGNED_SHORT,
                    (GLvoid *) 0);
        }
    }
    glDisableVertexAttribArray(shade_program->vertexPositionAttribute);
    glDisableVertexAttribArray(shade_program->vertexNormalAttribute);
    glDisableVertexAttribArray(shade_program->paramAttribute);
    glDisable(GL_POLYGON_OFFSET_FILL);
}
