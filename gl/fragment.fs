#version 330 core
out vec4 FragColor;
in vec2 out_tex_coords;

uniform sampler2D frag_texture;

void main(){
	FragColor = texture(frag_texture, out_tex_coords);
}
