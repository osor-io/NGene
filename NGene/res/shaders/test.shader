#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 uv;
layout(location = 2) in vec4 normal;

uniform mat4 mvp_matrix = mat4(1.0);

out DATA
{
	vec4 position;
	vec2 uv;
	vec4 normal;
} vs_out;

void main()
{
	gl_Position = mvp_matrix * position;
	vs_out.position = mvp_matrix * position;
	vs_out.normal = normal;
	vs_out.normal.z = 0.0f;
	vs_out.uv.x = uv.x;
	vs_out.uv.y = uv.y;
}






#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform sampler2D low_res_texture;

in DATA
{
	vec4 position;
	vec2 uv;
	vec4 normal;
} fs_in;

void main()
{
	//float intensity = 1.0 / length(fs_in.position.xy - light_pos);

	color = texture(low_res_texture, fs_in.uv); //+ fs_in.uv.xyxy;
	
	//color = fs_in.color * intensity;
	//color = fs_in.color;
	//color = vec4(fs_in.uv.xy, 0.0f, 1.0f);
}