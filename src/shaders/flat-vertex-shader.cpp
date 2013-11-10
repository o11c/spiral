#include "flat-vertex-shader.hpp"

#include <cstdio>

#include "../glue/error.hpp"
#include "../glue/gl_wrap.hpp"

FlatVertexShader::FlatVertexShader()
{
    shader = glCreateShader(GL_VERTEX_SHADER);

    const char *source =
#include "vertex-flat.glsl.h"
    ;
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint length = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
    if (length > 1)
    {
        GLchar buf[length];
        glGetShaderInfoLog(shader, length, &length, buf);
        printf("compiling flat vertex shader:\n%s\n\n", buf);
    }

    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
        barf();
}

FlatVertexShader::~FlatVertexShader()
{
    glDeleteShader(shader);
}
