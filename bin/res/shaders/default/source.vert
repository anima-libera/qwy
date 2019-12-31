
#version 460 core

uniform mat4 u_camera_matrix;

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_color;
layout(location = 2) in vec2 in_texture_coords;
layout(location = 3) in vec3 in_normal;

smooth out vec3 pass_color;
smooth out vec2 pass_texture_coords;
flat   out vec3 pass_normal;

void main()
{
	pass_color = in_color;
	pass_texture_coords = in_texture_coords;
	pass_normal = in_normal;

	gl_Position = u_camera_matrix * vec4(in_position, 1.0);
}
