#version 330 core

// inputs
layout (location = 0) in vec3 i_position;

uniform mat4 u_model_matrix;
uniform mat4 u_camera_matrix;

void main()
{
	gl_Position = u_camera_matrix * u_model_matrix * vec4(i_position, 1.0f);
}