#version 430 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 tex_coords;


uniform mat4 M_t;
uniform mat4 M_s;
uniform mat4 current_matrix;

out vec2 TexCoord;

void main()
{
	vec4 pos = vec4(in_position, 1.0);
	
	gl_Position = M_t * M_s * current_matrix * pos;
	TexCoord = tex_coords;
}