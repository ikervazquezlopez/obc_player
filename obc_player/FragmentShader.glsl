#version 430 core

//in vec2 TexCoord;

out vec4 color;

uniform sampler2D in_texture	;

void main(void)
{
	//color = texture(in_texture, TexCoord);
	color = vec4(0.6, 0.6, 0.6, 0.0);
}