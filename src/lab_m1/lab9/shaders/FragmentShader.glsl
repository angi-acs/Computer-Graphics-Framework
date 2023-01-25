#version 330

// Input
in vec2 texcoord;

// Uniform properties
uniform sampler2D texture_1;
uniform sampler2D texture_2;
// TODO(student): Declare various other uniforms
uniform float time;
uniform int globe;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // TODO(student): Calculate the out_color using the texture2D() function.
    out_color = vec4(1);
    vec2 copy = texcoord;

    if (globe == 1)
        copy.x += cos(time);

    vec4 color1 = texture2D(texture_1, copy);
    vec4 color2 = texture2D(texture_2, copy);
    vec3 color = mix(color1.xyz, color2.xyz, 0.7f);
    if (color1.a < 0.5f || color2.a < 0.5f) {
	    discard;
    }
    out_color = vec4(color, 1.f);
}
