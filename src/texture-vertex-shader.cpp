#include "texture-vertex-shader.hpp"

#include <cstdio>

#include "error.hpp"
#include "gl_wrap.hpp"

TextureVertexShader::TextureVertexShader()
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
        printf("compiling texture vertex shader:\n%s\n\n", buf);
    }

    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
        barf();
}

TextureVertexShader::~TextureVertexShader()
{
    glDeleteShader(shader);
}
