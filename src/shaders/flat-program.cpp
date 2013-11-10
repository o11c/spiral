#include "flat-program.hpp"

#include <cstdio>

#include "../glue/error.hpp"
#include "../glue/gl_wrap.hpp"
#include "../state.hpp"
#include "../glue/uniforms.hpp"

FlatProgram::FlatProgram(FlatVertexShader *v, SimpleFragmentShader *f)
: vs(v), fs(f)
{
    program = glCreateProgram();
    glAttachShader(program, vs->shader);
    glAttachShader(program, fs->shader);

    glLinkProgram(program);

    GLint length = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
    if (length > 1)
    {
        GLchar buf[length];
        glGetProgramInfoLog(program, length, &length, buf);
        printf("linking flat program:\n%s\n\n", buf);
    }

    GLint success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
        barf();

#define ATTRIBUTE(name, type)                               \
    name##Attribute = glGetAttribLocation(program, #name);  \
    if (name##Attribute == -1) barf();
#define UNIFORM(name, type)                                 \
    name##Uniform = glGetUniformLocation(program, #name);   \
    if (name##Uniform == -1) barf();
#include "vertex-flat.glsl.def"
#undef ATTRIBUTE
#undef UNIFORM
}

FlatProgram::~FlatProgram()
{
    glDeleteProgram(program);
}

void FlatProgram::load()
{
    mat4 ModelViewProjectionValue = encv::Projection * encv::ModelView;

    glUseProgram(program);
#define ATTRIBUTE(name, type)   /**/
#define UNIFORM(name, type)     load_uniform_##type(name##Uniform, name##Value);
#include "vertex-flat.glsl.def"
#undef ATTRIBUTE
#undef UNIFORM
}
