#version 330

// Input
    in vec3 frag_position;
    in vec3 frag_normal;
    in vec2 frag_texture;
    in vec3 frag_color;

    in vec3 world_position;
    in vec3 world_normal;

// Uniform properties
uniform sampler2D texture;

uniform vec3 light_direction;
uniform vec4 point_light_pos[20];
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;
uniform int spotlight;
uniform vec3 object_color[5];
uniform int obstacles;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    
    vec3 light = vec3(0,0,0);
    vec3 L = vec3(0.f, 0.f, -1.f);
    vec3 V = normalize( eye_position - world_position );
    vec3 H = normalize( L + V );
    vec3 R = reflect (-L, world_normal);

    float ambient_light = 0.5;

    float diffuse_light = 0;
    diffuse_light = material_kd * ambient_light * max (dot(world_normal, L), 0);

    float specular_light = 0;
    if (diffuse_light > 0)
    {
        specular_light = material_ks * ambient_light * 1 * pow(max(dot(V, R), 0), material_shininess);
    }

	light = ambient_light + diffuse_light * vec3(1,1,1) + specular_light * vec3(1,1,1);

    for (int i = 0; i < obstacles; i++) {
        vec3 light_position = point_light_pos[i].xyz;
        vec3 L = normalize( light_position - world_position );
        vec3 V = normalize( eye_position - world_position );
        vec3 H = normalize( L + V );
        vec3 R = reflect (-L, world_normal);

        float diffuse_light = 0;
        diffuse_light = material_kd * ambient_light * max (dot(world_normal, L), 0);

        float specular_light = 0;
        if (diffuse_light > 0)
        {
            specular_light = material_ks * ambient_light * 1 * pow(max(dot(V, R), 0), material_shininess);
        }

        float attenuation_factor = 1.f / pow( distance(light_position, world_position), 2);

        if (spotlight == 1) {
            float cut_off = radians(30);
            float spot_light = dot(-L, light_direction);
            float spot_light_limit = cos(cut_off);
            if (spot_light > cos(cut_off))
            {
	           float linear_att = (spot_light - spot_light_limit) / (1.f - spot_light_limit);
               float light_att_factor = pow(linear_att, 2);
               light += attenuation_factor * light_att_factor * (diffuse_light * vec3(1,1,1) + specular_light * vec3(1,1,1));
            }
        } else {
		    light += object_color[i%5] * attenuation_factor * (diffuse_light * vec3(1,1,1) + specular_light * vec3(1,1,1)); 
        }
     }

    out_color = vec4(1);
    vec4 color = texture2D(texture, frag_texture);
    if (color.a < 0.5f) {
	    discard;
    }
    out_color = vec4(vec4(light,1) * color);
}
