#include "state.hpp"

namespace encv
{
    mat4 ModelView;
    mat4 Projection;
    vec3 materialAmbient = {0.2, 0.2, 0.2};
    vec3 materialDiffuse = {0.1, 0.1, 0.1};
    vec3 materialSpecular = {1.0, 1.0, 1.0};
    /*float*/int materialShininess = 3;
    vec3 ambientLight = {0.2, 0.2, 0.2};
    vec3 lightPosition = {1000, 1000, 1000};
    vec3 lightColor = {1.0, 1.0, 1.0};
}
