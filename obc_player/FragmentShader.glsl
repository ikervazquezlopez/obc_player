#version 430 core

out vec4 color;

void main(void)
{
	color = vec4(0.0, gl_FragCoord.x, gl_FragCoord.z, 1.0);
}