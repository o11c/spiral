attribute vec3 vertexPosition;
attribute vec3 vertexNormal;
attribute vec2 param;

uniform mat4 ModelViewProjection;
uniform mat4 ModelView;
uniform mat3 NormalMatrix;
uniform vec3 materialAmbient;
uniform vec3 materialDiffuse;
uniform vec3 materialSpecular;
uniform float materialShininess;
uniform vec3 ambientLight;
uniform vec3 lightPosition;
uniform vec3 lightColor;

varying vec2 frag_param;
varying vec3 frag_normal;
varying vec4 frag_P;

void main()
{
    gl_Position = ModelViewProjection * vec4(vertexPosition, 1.0);
    frag_normal = NormalMatrix * vertexNormal;
    frag_param = param;
    frag_P = ModelView * vec4(vertexPosition, 1.0);
}
