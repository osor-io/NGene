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
uniform float tuning_diffuse_brightness;
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

vec4 sample_crt(vec2 uv) {

	vec2 scaled_uv = uv;
	scaled_uv *= uv_scalar;
	scaled_uv += uv_offset;

	vec2 scan_uv = (scaled_uv * crt_mask_scale) + crt_mask_offset;
	vec3 scan_tex_color = texture(scanlines_texture, scan_uv).rgb;
	scan_tex_color += tuning_scanline_brightness;
	scan_tex_color = mix(vec3(1, 1, 1), scan_tex_color, tuning_scanline_opacity);

	vec2 overscan_uv = (scaled_uv * tuning_overscan) - ((tuning_overscan - 1.0f) * 0.5);

	overscan_uv = overscan_uv - vec2(0.5, 0.5);
	float rsq = dot(overscan_uv, overscan_uv);
	overscan_uv = overscan_uv + (overscan_uv * (tuning_barrel_distortion * rsq)) + vec2(0.5, 0.5);


	vec3 game_color = texture(low_res_texture, overscan_uv).rgb;


	vec4 emmissive = vec4(game_color * scan_tex_color, 1);
	float dessaturation = dot(vec4(0.299, 0.587, 0.114, 0.0), emmissive);
	emmissive = mix(vec4(dessaturation, dessaturation, dessaturation, 1), emmissive, tuning_saturation);

	return emmissive;
}


void main()
{

	vec3 normal = normalize(fragment_in.normal);

	vec3 camera_direction = normalize(fragment_in.camera_direction);
	vec3 light_direction = normalize(fragment_in.light_direction);

	vec3 reflection = reflect(camera_direction, normal);

	float diffuse = clamp(dot(normal, light_direction), 0, 1);
	vec4 color_diffuse = vec4(0.175, 0.15, 0.2, 1) * diffuse * tuning_diffuse_brightness;

	vec3 half_vector = normalize(light_direction + camera_direction);
	float specular = clamp(dot(normal, half_vector), 0, 1);
	specular = pow(specular, tuning_specular_power);
	vec4 color_specular = vec4(0.25, 0.25, 0.25, 1) * specular * tuning_specular_brightness;

	float fresnel = 1.0 - dot(camera_direction, normal);
	fresnel = (fresnel * fresnel) * tuning_fresnel_brightness;
	vec4 color_fresnel = vec4(0.45, 0.4, 0.5, 1) * fresnel;

	color = sample_crt(fragment_in.uv) + color_diffuse + color_specular + color_fresnel;

	(color * mix(vec4(1, 1, 1, 1), fragment_in.color, tuning_dimming));
}