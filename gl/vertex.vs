#version 330 core
layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec2 in_tex_coords;

out vec2 out_tex_coords;
out vec3 my_coords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
	vec4 tmp = view * model * vec4(in_pos, 1.0f);
	my_coords = vec3(tmp.x, tmp.y, tmp.z);
	gl_Position = projection * tmp;
	out_tex_coords = in_tex_coords;
}
