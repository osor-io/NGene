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


uniform sampler2D image;
uniform vec2 resolution;
uniform vec2 direction;
uniform float tuning_blur;

vec4 blur9(sampler2D image, vec2 uv, vec2 resolution, vec2 direction) {
	vec4 color = vec4(0.0);
	vec2 off1 = vec2(1.3846153846) * direction;
	vec2 off2 = vec2(3.2307692308) * direction;
	color += texture2D(image, uv) * 0.2270270270;
	color += texture2D(image, uv + (off1 / resolution)) * 0.3162162162;
	color += texture2D(image, uv - (off1 / resolution)) * 0.3162162162;
	color += texture2D(image, uv + (off2 / resolution)) * 0.0702702703;
	color += texture2D(image, uv - (off2 / resolution)) * 0.0702702703;
	return color;
}

void main() {
	vec4 blurred_color = blur9(image, uv, resolution, direction);
	vec4 og_color = texture(image, uv);
	color = mix(og_color, blurred_color, tuning_blur);
}