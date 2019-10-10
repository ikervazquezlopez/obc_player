#version 430 core

in vec2 TexCoord;

out vec4 color;

uniform sampler2D in_texture;

void main(void)
{
	vec2 p = vec2(0.0, 0.0);
	color = texture(in_texture, TexCoord);
}