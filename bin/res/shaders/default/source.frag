
#version 460 core

uniform sampler2D u_texture_0;
//uniform vec3 u_light_direction;

in vec3 pass_color;
in vec2 pass_texture_coords;
in vec3 pass_normal;

out vec3 out_color;

void main()
{
	//float light = -dot(pass_normal, u_light_direction) * 0.5f + 0.5f;
	out_color = texture(u_texture_0, pass_texture_coords).rgb * pass_color;// * light;
}
