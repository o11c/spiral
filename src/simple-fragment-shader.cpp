#include "simple-fragment-shader.hpp"

#include <cstdio>

#include "error.hpp"
#include "gl_wrap.hpp"

SimpleFragmentShader::SimpleFragmentShader()
{
    shader = glCreateShader(GL_FRAGMENT_SHADER);

    const char *source =
#include "fragment-simple.glsl.h"
    ;
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint length = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
    if (length > 1)
    {
        GLchar buf[length];
        glGetShaderInfoLog(shader, length, &length, buf);
        printf("compiling simple fragment shader:\n%s\n\n", buf);
    }

    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
        barf();
}

SimpleFragmentShader::~SimpleFragmentShader()
{
    glDeleteShader(shader);
}