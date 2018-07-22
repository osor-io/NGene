#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 uv;
layout(location = 2) in vec4 normal;
layout(location = 3) in vec4 color;

uniform mat4 mvp_matrix = mat4(1.0);
uniform mat4 model_to_world_matrix = mat4(1.0);
uniform vec3 camera_position = vec3(0.0);
uniform vec3 tuning_light_position = vec3(0.0);

out DATA
{
	vec4 position;
	vec2 uv;
	vec3 normal;
	vec4 color;
	vec3 camera_direction;
	vec3 light_direction;
} vertex_out;

void main() {

	// Position
	{
		vertex_out.position = mvp_matrix * position;
		gl_Position = vertex_out.position;
	}

	// Color
	{
		vertex_out.color = color;
	}

	// UVs
	{
		vertex_out.uv = uv.xy;
	}

	// Normal
	{
		vertex_out.normal = normal.xyz;
	}

	// Camera & Light Direction
	{
		vec3 world_pos = (model_to_world_matrix * position).xyz;

		mat3 world_matrix_3 = mat3(model_to_world_matrix);
		mat3 transposed_world_matrix = transpose(world_matrix_3);

		vertex_out.camera_direction = transposed_world_matrix * (camera_position - world_pos);
		vertex_out.light_direction = transposed_world_matrix * (tuning_light_position - world_pos);
	}
}






#shader fragment
#version 330 core

out vec4 color;


uniform sampler2D low_res_texture;
uniform sampler2D scanlines_texture;


uniform vec2 uv_scalar;
uniform vec2 uv_offset;

uniform vec2 crt_mask_scale;
uniform vec2 crt_mask_offset;

uniform float tuning_overscan;
uniform float tuning_dimming;
uniform float tuning_saturation;
uniform float tuning_reflection_scalar;
uniform float tuning_barrel_distortion;
uniform float tuning_scanline_brightness;
uniform float tuning_scanline_opacity;
uniform float tuning_diff_brightness;
uniform float tuning_specular_brightness;
uniform float tuning_specular_power;
uniform float tuning_fresnel_brightness;



in DATA
{
	vec4 position;
	vec2 uv;
	vec3 normal;
	vec4 color;
	vec3 camera_direction;
	vec3 light_direction;
} fragment_in;

void main()
{
	color = texture(low_res_texture, fragment_in.uv);
	color = texture(scanlines_texture, fragment_in.uv);
	color = vec4(fragment_in.uv, 1.0, 1.0);
	color = vec4(fragment_in.normal, 1.0);
	color = fragment_in.color;
	color = vec4(fragment_in.camera_direction, 1.0);
	color = vec4(fragment_in.light_direction, 1.0);

	color = texture(low_res_texture, fragment_in.uv);
}