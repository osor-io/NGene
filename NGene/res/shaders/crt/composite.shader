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

uniform sampler2D current_frame;
uniform sampler2D previous_frame;

void main() {

	vec4 previous_color = texture(previous_frame, uv);
	vec4 current_color = texture(current_frame, uv);

	color = current_color;
}