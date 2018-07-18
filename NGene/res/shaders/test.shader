#shader vertex
void main() {
	gl_Position = vec4(0.5f, 0.5f, 0.5f, 0.5f);
}


#shader fragment
void main() {
	gl_FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0);
}