#version 330 core

struct material_{
    sampler2D diffuse;
    sampler2D specular;
    float shiness;
};

struct light_{
    vec3 pos;
    vec3 direction;
    float cut_off;
    float outer_cut_off;
    float constant;
    float linear;
    float quadratic;


    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 view_pos;
uniform material_ material;
uniform light_ light;

in vec3 normal;
in vec3 frag_pos;
in vec2 tex_coord;
out vec4 frag_color;

void main()
{
    // ambient
    vec3 ambient = light.ambient * texture(material.diffuse, tex_coord).rgb;
    // diffuse
    vec3 norm = normalize(normal);
    vec3 light_dir = normalize(light.pos - frag_pos);
    float diff = max(dot(light_dir, norm), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, tex_coord).rgb;
    // specular
    vec3 view_dir = normalize(view_pos - frag_pos);
    vec3 reflect_dir = reflect(-light_dir, norm);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shiness);
    vec3 specular = light.specular * spec * texture(material.specular, tex_coord).rgb;

    float distance = length(light.pos - frag_pos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    float theta = dot(light_dir, normalize(-light.direction));
    float epsilon = light.cut_off - light.outer_cut_off;
    float intensity = clamp((theta - light.outer_cut_off) / epsilon, 0.0, 1.0);
    diffuse *= intensity;
    specular *= intensity;
    vec3 result = ambient + diffuse + specular;
    frag_color = vec4(result, 1.0);
}