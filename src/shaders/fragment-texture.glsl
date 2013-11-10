varying vec4 frag_param;
varying vec3 frag_normal;
varying vec4 frag_P;

uniform sampler2D ambient_texture;
uniform sampler2D diffuse_texture;
uniform sampler2D specular_texture;
uniform float materialShininess;
uniform vec3 ambientLight;
uniform vec3 lightPosition;
uniform vec3 lightColor;

vec3 color_ambient(vec4 coord)
{
    return texture2DProj(ambient_texture, coord).rgb;
}

vec3 color_diffuse(vec4 coord)
{
    return texture2DProj(diffuse_texture, coord).rgb;
}

vec3 color_specular(vec4 coord)
{
    return texture2DProj(specular_texture, coord).rgb;
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
