#ifndef STATE_HPP
#define STATE_HPP

#include "matrix.hpp"

namespace encv
{
    extern mat4 ModelView;
    extern mat4 Projection;
    extern vec3 materialAmbient;
    extern vec3 materialDiffuse;
    extern vec3 materialSpecular;
    extern float materialShininess;
    extern vec3 ambientLight;
    extern vec3 lightPosition;
    extern vec3 lightColor;
}

#endif //STATE_HPP
