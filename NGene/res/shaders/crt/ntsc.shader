#shader vertex
#version 330 core


out vec2 uv;

// Fullscreen trick to only have to render a triangle.
void main() {


	uv.x = (gl_VertexID == 2) ? 2.0 : 0.0;
	uv.y = (gl_VertexID == 1) ? 2.0 : 0.0;

	gl_Position = vec4(uv*vec2(2.0, -2.0) + vec2(-1.0, 1.0), 1.0, 1.0);

	uv.y = 1 - uv.y;
}




#shader fragment
#version 330 core

in vec2 uv;

out vec4 color;


uniform sampler2D lut_texture; // Look Up Texture for color grading
uniform sampler2D game_frame; // Our rendered image from the game


uniform float tuning_strength; // From [0,1]: Defines how strong the color grading effect is from original pixel to full color graded

uniform float lut_resolution; // The resolution in one axis of the Look Up Texture, normally 16, 32


void main() {

	vec4 clean_color = texture(game_frame, uv);

	float res = lut_resolution;
	float rcp_res = 1.0f / res;
	float res_sq = res * res;
	float rcp_res_sq = 1.0f / res_sq;
	float half_rcp_res_sq = 0.5f * rcp_res_sq;
	float half_rcp_res = 0.5f * rcp_res;

	float res_minus_one = res - 1.0f;
	float res_minus_one_over_res = res_minus_one / res;
	float res_minus_one_over_res_sq = res_minus_one / res_sq;

	vec2 gradient_uv_low;
	vec2 gradient_uv_high;

	float blue_low = floor(clean_color.b * res_minus_one);
	float blue_high = ceil(clean_color.b * res_minus_one);
	float blue_alpha = ((clean_color.b * res_minus_one) - blue_low);

	gradient_uv_low.x = (blue_low * rcp_res) + clean_color.r * res_minus_one_over_res_sq + half_rcp_res_sq;
	gradient_uv_low.y = clean_color.g * res_minus_one_over_res + half_rcp_res;

	gradient_uv_high.x = (blue_high * rcp_res) + clean_color.r * res_minus_one_over_res_sq + half_rcp_res_sq;
	gradient_uv_high.y = clean_color.g * res_minus_one_over_res + half_rcp_res;

	vec4 postgradient_color_low = texture(lut_texture, gradient_uv_low);
	vec4 postgradient_color_high = texture(lut_texture, gradient_uv_high);

	vec4 postgradient_color = mix(postgradient_color_low, postgradient_color_high, blue_alpha);

	color = mix(clean_color, postgradient_color, tuning_strength );
}