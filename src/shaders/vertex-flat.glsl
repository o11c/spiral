attribute vec3 vertexPosition;
attribute vec4 param;

uniform mat4 ModelViewProjection;

varying vec4 frag_color;

void main()
{
    gl_Position = ModelViewProjection * vec4(vertexPosition, 1.0);
    float t = param.s / param.q * radians(360);
    float u = param.t / param.q * radians(360);
    float cu = (cos(u) + 1.0) / 2.0;
    float st = (sin(t) + 1.0) / 2.0;
    float ct = (cos(t) + 1.0) / 2.0;
    frag_color = vec4(cu, st, ct, 1.0);
}
