#version 330

// Input
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 point_light_pos[2];
uniform vec3 light_direction;
// uniform vec3 light_position;
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;
uniform int spotlight;

// TODO(student): Declare any other uniforms

uniform vec3 object_color;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    vec3 light_position = point_light_pos[0];
    vec3 light_position2 = point_light_pos[1];
    vec3 L = normalize( light_position - world_position );
    vec3 L2 = normalize( light_position2 - world_position );
    vec3 V = normalize( eye_position - world_position );
    vec3 H = normalize( L + V );
    vec3 H2 = normalize( L2 + V );
    vec3 R = reflect (-L, world_normal);
    vec3 R2 = reflect (-L2, world_normal);

    // TODO(student): Define ambient, diffuse and specular light components
    float ambient_light = 0.25;

    float diffuse_light = 0;
    float diffuse_light2 = 0;
    diffuse_light = material_kd * ambient_light * max (dot(world_normal, L), 0);
    diffuse_light2 = material_kd * ambient_light * max (dot(world_normal, L2), 0);

    float specular_light = 0;
    float specular_light2 = 0;
    // It's important to distinguish between "reflection model" and
    // "shading method". In this shader, we are experimenting with the Phong
    // (1975) and Blinn-Phong (1977) reflection models, and we are using the
    // Phong (1975) shading method. Don't mix them up!
    if (diffuse_light > 0)
    {
        specular_light = material_ks * ambient_light * 1 * pow(max(dot(V, R), 0), material_shininess);
    }

    if (diffuse_light2 > 0)
    {
        specular_light2 = material_ks * ambient_light * 1 * pow(max(dot(V, R2), 0), material_shininess);
    }

    // TODO(student): If (and only if) the light is a spotlight, we need to do
    // some additional things.
    float light = 0.f;
    float attenuation_factor = 1.f / pow( distance(light_position, world_position), 2);
    float attenuation_factor2 = 1.f / pow( distance(light_position2, world_position), 2);

    if (spotlight == 1) {
        float cut_off = radians(30);
        float spot_light = dot(-L, light_direction);
        float spot_light_limit = cos(cut_off);
        if (spot_light > cos(cut_off))
        {
	       float linear_att = (spot_light - spot_light_limit) / (1.f - spot_light_limit);
           float light_att_factor = pow(linear_att, 2);
           light = ambient_light + attenuation_factor * light_att_factor * (diffuse_light + specular_light);
        } else {
            light = ambient_light;
        }
    } else {
		light = ambient_light + attenuation_factor * (diffuse_light + specular_light)
                              + attenuation_factor2 * (diffuse_light2 + specular_light2);
    }

    // TODO(student): Compute the total light. You can just add the components
    // together, but if you're feeling extra fancy, you can add individual
    // colors to the light components. To do that, pick some vec3 colors that
    // you like, and multiply them with the respective light components.

    // TODO(student): Write pixel out color
    out_color = vec4(1);
    out_color = vec4(object_color * light, 1.0);
}
