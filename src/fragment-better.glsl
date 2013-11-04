varying vec4 frag_param;
varying vec3 frag_normal;
varying vec4 frag_P;

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

vec4 get_tex_color(vec4 coord)
{
    if (true)
    {
        float t = frag_param.s / coord.q * radians(360);
        float u = frag_param.t / coord.q * radians(360);
        float cu = (cos(u) + 1.0) / 2.0;
        float st = (sin(t) + 1.0) / 2.0;
        float ct = (cos(t) + 1.0) / 2.0;
        return vec4(cu, st, ct, 1.0);
    }
    else
    {
        return vec4(0.0, coord.st / coord.q, 1.0);
    }
}

void main()
{
    vec3 Iambient = materialAmbient * ambientLight;
    vec3 N = normalize(frag_normal);
    vec3 L = normalize(lightPosition - frag_P.xyz);
    float cos_theta = dot(L, N);
    vec3 Idiffuse = materialDiffuse * lightColor * max(0.0, cos_theta);
    vec3 Ispecular = vec3(0.0, 0.0, 0.0);
    if (cos_theta > 0.0)
    {
        vec3 R = reflect(-L, N);
        vec3 V = normalize(-frag_P.xyz);
        float cos_alpha = dot(R, V);
        Ispecular = materialSpecular * lightColor * pow(max(0.0, cos_alpha), materialShininess);
    }
    vec3 I = Iambient + Idiffuse + Ispecular;
    gl_FragColor = vec4(I, 1.0) * get_tex_color(frag_param);
}
