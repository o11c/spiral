attribute vec3 vertexPosition;
attribute vec2 param;

uniform mat4 ModelViewProjection;

varying vec4 frag_color;

void main()
{
    gl_Position = ModelViewProjection * vec4(vertexPosition, 1.0);
    float t = param.x;
    float u = param.y;
    frag_color = vec4(cos(u), sin(t), cos(t), 1.0);
}
