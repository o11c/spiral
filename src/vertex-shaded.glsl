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

varying vec4 frag_color;

void main()
{
    float t = param.x;
    float u = param.y;
    float cu = (cos(u) + 1.0) / 2.0;
    float st = (sin(t) + 1.0) / 2.0;
    float ct = (cos(t) + 1.0) / 2.0;
    vec3 wannabe_texture = vec3(cu, st, ct);

    gl_Position = ModelViewProjection * vec4(vertexPosition, 1.0);
    vec3 Iambient = materialAmbient * ambientLight;
    vec3 N = normalize(NormalMatrix * vertexNormal);
    vec4 P = ModelView * vec4(vertexPosition, 1.0);
    vec3 L = normalize(lightPosition - P.xyz);
    float cos_theta = dot(L, N);
    vec3 Idiffuse = materialDiffuse * lightColor * max(0.0, cos_theta);
    vec3 Ispecular = vec3(0.0, 0.0, 0.0);
    if (cos_theta > 0.0)
    {
        vec3 R = reflect(-L, N);
        vec3 V = normalize(-P.xyz);
        float cos_alpha = dot(R, V);
        Ispecular = materialSpecular * lightColor * pow(max(0.0, cos_alpha), materialShininess);
    }
    vec3 I = Iambient + Idiffuse + Ispecular;
    frag_color = vec4(I * wannabe_texture, 1.0);
}
