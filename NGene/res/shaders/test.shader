#shader vertex
#version 130

attribute vec2 position;

void main() {
	gl_Position = vec4(position, 0.0, 1.0);
}



#shader fragment
#version 130

uniform vec3 triangleColor;

void main() {
	gl_FragColor = vec4(triangleColor, 1.0);
}