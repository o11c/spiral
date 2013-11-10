attribute vec3 vertexPosition;
attribute vec3 vertexNormal;
attribute vec4 param;

uniform mat4 ModelViewProjection;
uniform mat4 ModelView;
uniform mat3 NormalMatrix;
uniform mat4 TextureMatrix;

varying vec4 frag_param;
varying vec3 frag_normal;
varying vec4 frag_P;

void main()
{
    gl_Position = ModelViewProjection * vec4(vertexPosition, 1.0);
    frag_normal = NormalMatrix * vertexNormal;
    frag_param = TextureMatrix * param;
    frag_P = ModelView * vec4(vertexPosition, 1.0);
}
