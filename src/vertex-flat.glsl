attribute vec3 vertexPosition;

uniform mat4 ModelViewProjection;
uniform vec3 flat_color;

varying vec4 frag_color;

void main()
{
    gl_Position = ModelViewProjection * vec4(vertexPosition, 1.0);
    frag_color = vec4(flat_color, 1.0);
}
