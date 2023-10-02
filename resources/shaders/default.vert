#version 330 core

layout (location = 0) in vec3 t_position;
layout (location = 1) in vec3 t_normal;
layout (location = 2) in vec3 t_color;

out vec3 current_position;
out vec3 normal;
out vec3 color;

uniform mat4 camera_matrix;
uniform mat4 model_matrix;

void main()
{
	current_position = vec3(model_matrix * vec4(t_position, 1.0f));
	normal = t_normal;
	color = t_color;
	gl_Position = camera_matrix * vec4(current_position, 1.0);
}