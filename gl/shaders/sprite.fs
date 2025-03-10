#version 330 core
uniform sampler2D sprite_texture;
out vec4 FragColor;

in vec2 out_tex_coords;
in vec3 my_coords;

uniform float fog_start;
uniform float fog_end;
uniform vec4 fog_color;

void main(){
	float my_length = sqrt(dot(my_coords, my_coords));
	float value = (fog_end - my_length) / (fog_end - fog_start);

	if(value < 0.0f)
		value = 0.0f;
	if(value > 1.0f)
		value = 1.0f;

	FragColor = texture(sprite_texture, out_tex_coords);

	float old_value = FragColor.w;

	FragColor = FragColor * value + fog_color * (1.0f - value);

	FragColor.w = old_value;
}
