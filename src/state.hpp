#ifndef STATE_HPP
#define STATE_HPP

class mat4;
class vec3;
class sampler2D;

namespace encv
{
    extern mat4 View; // View is just ModelView at the beginning of time
    extern mat4 ModelView;
    extern mat4 Projection;
    extern mat4 TextureMatrix;
    extern vec3 materialAmbient;
    extern vec3 materialDiffuse;
    extern vec3 materialSpecular;
    extern /*float*/int materialShininess;
    extern vec3 ambientLight;
    extern vec3 lightPosition;
    extern vec3 lightColor;

    extern sampler2D *ambient_texture;
    extern sampler2D *diffuse_texture;
    extern sampler2D *specular_texture;
}

#endif //STATE_HPP
