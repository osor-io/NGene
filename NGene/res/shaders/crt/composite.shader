#shader vertex
#version 330 core


out vec2 uv;

// Fullscreen trick to only have to render a triangle.
void main() {


	uv.x = (gl_VertexID == 2) ? 2.0 : 0.0;
	uv.y = (gl_VertexID == 1) ? 2.0 : 0.0;

	gl_Position = vec4(uv*vec2(2.0, -2.0) + vec2(-1.0, 1.0), 1.0, 1.0);

	//uv.y = 1 - uv.y;
}




#shader fragment
#version 330 core

in vec2 uv;

out vec4 color;

uniform sampler2D current_frame;
uniform sampler2D previous_frame;

uniform sampler2D ntsc_artifact_tex;

uniform float ntsc_lerp = 0.5;
uniform float tuning_sharp = 0.5;
uniform vec4 tuning_persistence = vec4(0.5, 0.5, 0.5, 0.5);
uniform float tuning_bleed = 0.5;
uniform float tuning_ntsc = 0.5;

float SharpWeight[3] = float[]( 1.0, -0.3162277, 0.1 );

vec2 rcp_screen_width = vec2(1.0 / 256.0, 0.0);
vec2 rcp_screen_height = vec2(0.0, 1.0 / 224.0);


float brightness(vec4 color) {
	return dot(color, vec4(0.299, 0.587, 0.144, 0.0));
}

void main() {


	vec4 ntsc_artifact_1 = texture(ntsc_artifact_tex, uv);
	vec4 ntsc_artifact_2 = texture(ntsc_artifact_tex, uv + rcp_screen_height);
	vec4 ntsc_artifact = mix(ntsc_artifact_1, ntsc_artifact_2, ntsc_lerp);

	vec2 left_uv = uv - rcp_screen_width;
	vec2 right_uv = uv + rcp_screen_width;

	vec4 current_left = texture(current_frame, left_uv);
	vec4 current_local = texture(current_frame, uv);
	vec4 current_right = texture(current_frame, right_uv);

	vec4 tuned_ntsc = ntsc_artifact * tuning_ntsc;

	vec4 previous_left = texture(previous_frame, left_uv);
	vec4 previous_local = texture(previous_frame, uv);
	vec4 previous_right = texture(previous_frame, right_uv);
	
	current_local = clamp(
		current_local + (((current_left - current_local) + (current_right - current_local)) * tuned_ntsc)
		, 0.0, 1.0);

	float current_brightness = brightness(current_local);

	float offset = 0.0;


	for (int i = 0; i < 3; ++i) {
		vec2 step_size = (rcp_screen_width * (float(i + 1)));
	
		vec4 neighbor_left = texture(current_frame, uv - step_size);
		vec4 neighbor_right = texture(current_frame, uv + step_size);

		float brightness_left = brightness(neighbor_left);
		float brightness_right = brightness(neighbor_right);

		offset +=
			((current_brightness - brightness_left) + (current_brightness - brightness_right))
			* SharpWeight[i];
	}

	current_local = clamp(
		current_local + (offset * tuning_sharp * mix(vec4(1,1,1,1), ntsc_artifact, tuning_ntsc)),
		0.0, 1.0);

	current_local = clamp(
		max(current_local,
			tuning_persistence * (1.0 / (1.0 + (2.0 * tuning_bleed))) * (previous_local + ((previous_left + previous_right) * tuning_bleed))),
		0.0, 1.0);

	color = current_local;
}