#version 330 core

// inputs
layout (location = 0) in vec3 i_position;
layout (location = 1) in vec3 i_normal;
layout (location = 2) in vec3 i_color;

// NOTE: keep these names consistent between shaders
// NOTE: these variables are passed to fragment shader
out vec3 c_current_position;
out vec3 c_normal;
out vec3 c_color;

uniform mat4 u_camera_matrix;
uniform mat4 u_translation;
uniform mat4 u_rotation;
uniform mat4 u_scale;
uniform mat4 u_model_matrix;

void main()
{
  c_current_position = vec3(u_model_matrix * u_translation * -u_rotation * u_scale * vec4(i_position, 1.0f));
  c_normal = i_normal;
  c_color = i_color;
  gl_Position = u_camera_matrix * vec4(c_current_position, 1.0);
}