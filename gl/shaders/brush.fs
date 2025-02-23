#version 330 core
uniform sampler2D brush_texture;
out vec4 FragColor;

in vec2 out_tex_coords;
in vec3 my_coords;
in vec3 out_normal;

uniform mat4 model;
uniform mat4 view;

uniform float fog_start;
uniform float fog_end;
uniform vec4 fog_color;

void main(){
	float my_length = length(my_coords);
	float value = (fog_end - my_length) / (fog_end - fog_start);

	if(value < 0.0f)
		value = 0.0f;
	if(value > 1.0f)
		value = 1.0f;

	//value = 1.0f;

	FragColor = texture(brush_texture, out_tex_coords) * value + fog_color * (1.0f - value);
	FragColor.w = 1.0f;
}
