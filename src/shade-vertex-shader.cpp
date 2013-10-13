#include "shade-vertex-shader.hpp"

#include <cstdio>

#include "error.hpp"
#include "gl_wrap.hpp"

ShadeVertexShader::ShadeVertexShader()
{
    shader = glCreateShader(GL_VERTEX_SHADER);

    const char *source =
#include "vertex-shaded.glsl.h"
    ;
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint length = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
    if (length > 1)
    {
        GLchar buf[length];
        glGetShaderInfoLog(shader, length, &length, buf);
        printf("compiling shaded vertex shader:\n%s\n\n", buf);
    }

    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
        barf();
}

ShadeVertexShader::~ShadeVertexShader()
{
    glDeleteShader(shader);
}
