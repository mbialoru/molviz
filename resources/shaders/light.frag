#version 330 core

out vec4 fragment_color;

uniform vec4 light_color;

void main()
{
	fragment_color = light_color;
}