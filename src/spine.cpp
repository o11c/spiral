#include "spine.hpp"

#include <cmath>

#include "gl_wrap.hpp"
#include "vector.hpp"

#define a 100
#define b 40
#define p 2
#define q 5
#define N 40

static
vec3 C(float t)
{
    float x = (a + b * cos(q * t)) * cos(p * t);
    float y = (a + b * cos(q * t)) * sin(p * t);
    float z = b * sin(q * t);
    return {x, y, z};
}

void Spine::draw()
{
    program->load();

    GLuint buf;
    {
        glGenBuffers(1, &buf);
        vec3 points[N];
        for (int i = 0; i < N; ++i)
        {
            float t = 2 * M_PI * i / N;
            points[i] = C(t);
        }
        glBindBuffer(GL_ARRAY_BUFFER, buf);
        glBufferData(GL_ARRAY_BUFFER,
                sizeof(points), points,
                GL_STATIC_DRAW);
    }
    {
        glEnableVertexAttribArray(program->vertexPositionAttribute);
        glBindBuffer(GL_ARRAY_BUFFER, buf);
        glVertexAttribPointer(program->vertexPositionAttribute, 3, GL_FLOAT, GL_FALSE,
                0, (GLvoid*) 0);
        glDrawArrays(GL_LINE_LOOP, 0, N);
        glDisableVertexAttribArray(program->vertexPositionAttribute);
    }
    {
        glDeleteBuffers(1, &buf);
    }
}
