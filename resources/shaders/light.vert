#version 330 core

layout (location = 0) in vec3 t_position;

uniform mat4 model_matrix;
uniform mat4 camera_matrix;

void main()
{
	gl_Position = camera_matrix * model_matrix * vec4(t_position, 1.0f);
}