#include "shade-program.hpp"

#include <cstdio>

#include "../glue/error.hpp"
#include "../glue/gl_wrap.hpp"
#include "../glue/uniforms.hpp"

#include "../state.hpp"

ShadeProgram::ShadeProgram(ShadeVertexShader *v, BetterFragmentShader *f)
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
        printf("linking shaded program:\n%s\n\n", buf);
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
#include "vertex-shaded.glsl.def"
#undef ATTRIBUTE
#undef UNIFORM
}

ShadeProgram::~ShadeProgram()
{
    glDeleteProgram(program);
}

void ShadeProgram::load()
{
    mat4 ModelViewProjectionValue = encv::Projection * encv::ModelView;
    auto& ModelViewValue = encv::ModelView;
    mat3 NormalMatrixValue = encv::ModelView.get_normal();
    auto& materialAmbientValue = encv::materialAmbient;
    auto& materialDiffuseValue = encv::materialDiffuse;
    auto& materialSpecularValue = encv::materialSpecular;
    float materialShininessValue = /*int*/encv::materialShininess;
    auto& ambientLightValue = encv::ambientLight;
    vec3 lightPositionValue = (encv::View * vec4(encv::lightPosition, 1.0)).xyz();
    auto& lightColorValue = encv::lightColor;

    glUseProgram(program);
#define ATTRIBUTE(name, type)   /**/
#define UNIFORM(name, type)     load_uniform_##type(name##Uniform, name##Value);
#include "vertex-shaded.glsl.def"
#undef ATTRIBUTE
#undef UNIFORM
}

