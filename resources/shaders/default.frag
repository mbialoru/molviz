#version 330 core

out vec4 fragment_color;

in vec3 t_current_position;
in vec3 t_normal;
in vec3 t_color; // unused for now

uniform vec4 t_light_color;
uniform vec3 t_light_position;
uniform vec3 t_camera_position;

vec4 point_light()
{
	// used in two variables so I calculate it here to not have to do it twice
	vec3 light_vector = t_light_position - t_current_position;

	// intensity of light with respect to distance
	float dist = length(light_vector);
	float a = 3.0;
	float b = 0.7;
	float intensity = 1.0f / (a * dist * dist + b * dist + 1.0f);

	// ambient lighting
	float ambient_light = 0.20f;

	// diffuse lighting
	vec3 normal = normalize(t_normal);
	vec3 light_direction = normalize(light_vector);
	float diffuse = max(dot(normal, light_direction), 0.0f);

	// specular lighting
	float specular_light = 0.50f;
	vec3 view_direction = normalize(t_camera_position - t_current_position);
	vec3 reflection_direction = reflect(-light_direction, normal);
	float specular_amount = pow(max(dot(view_direction, reflection_direction), 0.0f), 16);
	float specular = specular_amount * specular_light;

	return (vec4(t_color, 1.0f) * (diffuse * intensity + ambient_light) +
	vec4(t_color, 1.0f).x * specular * intensity) * t_light_color;
}

vec4 direct_light()
{
	// ambient lighting
	float ambient_light = 0.20f;

	// diffuse lighting
	vec3 normal = normalize(t_normal);
	vec3 light_direction = normalize(vec3(1.0f, 1.0f, 0.0f));
	float diffuse = max(dot(normal, light_direction), 0.0f);

	// specular lighting
	float specular_light = 0.50f;
	vec3 view_direction = normalize(t_camera_position - t_current_position);
	vec3 reflection_direction = reflect(-light_direction, normal);
	float specular_amount = pow(max(dot(view_direction, reflection_direction), 0.0f), 16);
	float specular = specular_amount * specular_light;

	return (vec4(t_color, 1.0f) * (diffuse + ambient_light) + vec4(t_color, 1.0f) * specular) * t_light_color;
}

vec4 spot_light()
{
	// controls how big the area that is lit up is
	float outer_cone = 0.90f;
	float inner_cone = 0.95f;

	// ambient lighting
	float ambient_light = 0.20f;

	// diffuse lighting
	vec3 normal = normalize(t_normal);
	vec3 light_direction = normalize(t_light_position - t_current_position);
	float diffuse = max(dot(normal, light_direction), 0.0f);

	// specular lighting
	float specular_light = 0.50f;
	vec3 view_direction = normalize(t_camera_position - t_current_position);
	vec3 reflection_direction = reflect(-light_direction, normal);
	float specular_amount = pow(max(dot(view_direction, reflection_direction), 0.0f), 16);
	float specular = specular_amount * specular_light;

	// calculates the intensity of the t_current_position based on its angle to the center of the light cone
	float angle = dot(vec3(0.0f, -1.0f, 0.0f), -light_direction);
	float intensity = clamp((angle - outer_cone) / (inner_cone - outer_cone),
							0.0f, 1.0f);

	return (vec4(t_color, 1.0f) * (diffuse * intensity + ambient_light) + vec4(t_color, 1.0f).x * specular * intensity) * t_light_color;
}

void main()
{
	fragment_color = vec4(t_color, 1.0f);//direct_light();
}