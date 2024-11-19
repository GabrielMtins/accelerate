#version 330 core
uniform sampler2D model_texture;
out vec4 FragColor;

in vec2 out_tex_coords;
in vec3 my_coords;

void main(){
	//FragColor = texture(model_texture, out_tex_coords);
	FragColor = texture(model_texture, out_tex_coords) / (dot(my_coords, my_coords) / 2.0f);
	//FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f) / (dot(my_coords, my_coords) / 2.0f);
	FragColor.w = 1.0f;
}
