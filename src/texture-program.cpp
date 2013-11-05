#include "texture-program.hpp"

#include <cstdio>

#include "error.hpp"
#include "gl_wrap.hpp"
#include "uniforms.hpp"

#include "state.hpp"

TextureProgram::TextureProgram(TextureVertexShader *v, TextureFragmentShader *f)
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
        printf("linking texture program:\n%s\n\n", buf);
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
#include "vertex-texture.glsl.def"
#undef ATTRIBUTE
#undef UNIFORM
}

TextureProgram::~TextureProgram()
{
    glDeleteProgram(program);
}

void TextureProgram::load()
{
    mat4 ModelViewProjectionValue = encv::Projection * encv::ModelView;
    auto& ModelViewValue = encv::ModelView;
    mat3 NormalMatrixValue = encv::ModelView.get_normal();
    auto& materialAmbientValue = encv::materialAmbient;
    auto& materialDiffuseValue = encv::materialDiffuse;
    auto& materialSpecularValue = encv::materialSpecular;
    float materialShininessValue = /*int*/encv::materialShininess;
    auto& ambientLightValue = encv::ambientLight;
    auto& lightPositionValue = encv::lightPosition;
    auto& lightColorValue = encv::lightColor;

    glUseProgram(program);
#define ATTRIBUTE(name, type)   /**/
#define UNIFORM(name, type)     load_uniform_##type(name##Uniform, name##Value);
#include "vertex-texture.glsl.def"
#undef ATTRIBUTE
#undef UNIFORM
}

