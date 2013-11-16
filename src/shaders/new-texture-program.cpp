#include "new-texture-program.hpp"

#include <cstdio>

#include "../glue/error.hpp"
#include "../glue/gl_wrap.hpp"
#include "../glue/uniforms.hpp"

NewTextureProgram::NewTextureProgram(TextureVertexShader *v, TextureFragmentShader *f)
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
#include "fragment-texture.glsl.def"
#undef ATTRIBUTE
#undef UNIFORM
}

NewTextureProgram::~NewTextureProgram()
{
    glDeleteProgram(program);
}

void NewTextureProgram::load(Context& ctx, Material& mat)
{
    mat4 ModelViewProjectionValue = ctx.Projection * ctx.ModelView;
    auto& ModelViewValue = ctx.ModelView;
    mat3 NormalMatrixValue = ctx.ModelView.get_normal();
    auto TextureMatrixValue = mat4(); // ctx.TextureMatrix;
    auto& ambient_textureValue = *mat.ambient;
    auto& diffuse_textureValue = *mat.diffuse;
    auto& specular_textureValue = *mat.specular;
    //auto& normal_textureValue = *mat.normal;
    float materialShininessValue = /*int*/mat.shininess;
    auto ambientLightValue = vec3(1.0, 1.0, 1.0); //ctx.ambientLight;
    vec3 lightPositionValue = ctx.light.position.xyz;
    auto& lightColorValue = ctx.light.color.xyz;

    glUseProgram(program);
#define ATTRIBUTE(name, type)   /**/
#define UNIFORM(name, type)     load_uniform_##type(name##Uniform, name##Value);
#include "vertex-texture.glsl.def"
#include "fragment-texture.glsl.def"
#undef ATTRIBUTE
#undef UNIFORM
}

