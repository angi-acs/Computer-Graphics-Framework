#version 330

// Input
    layout(location = 0) in vec3 v_position;
    layout(location = 1) in vec3 v_normal;
    layout(location = 2) in vec3 v_texture;
    layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 car_pos;

// Output
    out vec3 frag_position;
    out vec3 frag_normal;
    out vec3 frag_texture;
    out vec3 frag_color;

void main()
{
    frag_position = v_position;
    frag_normal = v_normal;
    frag_texture = v_texture;
    frag_color = v_color;

    vec3 world_pos = (Model * vec4(v_position, 1.0)).xyz;
    float dist = distance(car_pos, world_pos);
    world_pos.y -= dist * dist * 0.003;

    gl_Position = Projection * View * vec4(world_pos, 1.0);

    //gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
