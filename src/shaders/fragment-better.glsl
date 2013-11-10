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

vec3 phony_color(vec2 st)
{
    float s = st.s;
    float t = st.t;

    bool line = false;
    if (mod(s * 18.0, 1.0) < 0.1)
        line = true;
    if (mod(t * 18.0, 1.0) < 0.1)
        line = true;

    if (true)
    {
        float s_ = s * radians(360);
        float t_ = t * radians(360);
        float ct = (cos(t_) + 1.0) / 2.0;
        float ss = (sin(s_) + 1.0) / 2.0;
        float cs = (cos(s_) + 1.0) / 2.0;
        return vec3(ct, ss, cs);
    }
    else
    {
        return vec3(float(line), s, t);
    }
}

vec3 color_ambient(vec4 coord)
{
    return materialAmbient * phony_color(coord.st / coord.q);
}

vec3 color_diffuse(vec4 coord)
{
    return materialDiffuse * phony_color(coord.st / coord.q);
}

vec3 color_specular(vec4 coord)
{
    return materialSpecular;
}

void main()
{
    vec3 Iambient = color_ambient(frag_param) * ambientLight;
    vec3 N = normalize(frag_normal);
    vec3 L = normalize(lightPosition - frag_P.xyz);
    float cos_theta = dot(L, N);
    vec3 Idiffuse = color_diffuse(frag_param) * lightColor * max(0.0, cos_theta);
    vec3 Ispecular = vec3(0.0, 0.0, 0.0);
    if (cos_theta > 0.0)
    {
        vec3 R = reflect(-L, N);
        vec3 V = normalize(-frag_P.xyz);
        float cos_alpha = dot(R, V);
        Ispecular = color_specular(frag_param) * lightColor * pow(max(0.0, cos_alpha), materialShininess);
    }
    vec3 color = Iambient + Idiffuse + Ispecular;
    gl_FragColor = vec4(color, 1.0);
}
