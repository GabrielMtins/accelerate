#version 330 core
layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec2 in_tex_coords;

uniform vec4 offset_tex_coords;
uniform mat4 model;

out vec2 out_tex_coords;

void main(){
	gl_Position = model * vec4(in_pos, 1.0f);
	gl_Position.y *= -1.0f;
	gl_Position.z = 0.0f;

	out_tex_coords = vec2(
			in_tex_coords.x * offset_tex_coords.z + offset_tex_coords.x,
			in_tex_coords.y * offset_tex_coords.w + offset_tex_coords.y
	);
}
