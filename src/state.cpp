#include "state.hpp"

#include "matrix.hpp"
#include "texture.hpp"

namespace encv
{
    mat4 ModelView;
    mat4 Projection;
    mat4 TextureMatrix;
    vec3 materialAmbient = {0.2, 0.2, 0.2};
    vec3 materialDiffuse = {0.3, 0.3, 0.3};
    vec3 materialSpecular = {0.8, 0.8, 0.8};
    /*float*/int materialShininess = 12;
    vec3 ambientLight = {0.5, 0.5, 0.5};
    vec3 lightPosition = {1000, 1000, 1000};
    vec3 lightColor = {1.0, 1.0, 1.0};

    sampler2D *ambient_texture = nullptr;
    sampler2D *diffuse_texture = nullptr;
    sampler2D *specular_texture = nullptr;
}
