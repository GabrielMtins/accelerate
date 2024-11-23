#include "Shader.hpp"
#include "glad/glad.h"

#include <fstream>

namespace acc {

Shader::Shader(void){
	loaded = false;
	id = 0;
}

Shader::Shader(const std::string& name){
	setName(name);
	loaded = false;
	id = 0;
}

Shader::Shader(const std::string &name, const std::string &vertex_filename, const std::string& fragment_filename){
	setName(name);
	loaded = false;
	id = 0;

	loadFile(vertex_filename, fragment_filename);
}

bool Shader::loadShader(const char *vertex_src, const char *fragment_src){
	unsigned int vertex_shader = 0, fragment_shader = 0;
	loaded = true;

	{
		vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, &vertex_src, NULL);
		glCompileShader(vertex_shader);

		int sucess;
		char log[512];
		glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &sucess);

		if(!sucess){
			glGetShaderInfoLog(vertex_shader, 512, NULL, log);
			fprintf(stderr, "error loading vertex shader\n%s", log);
			loaded = false;
		}
	}

	{
		fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, &fragment_src, NULL);
		glCompileShader(fragment_shader);

		int sucess;
		char log[512];
		glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &sucess);

		if(!sucess){
			glGetShaderInfoLog(fragment_shader, 512, NULL, log);
			fprintf(stderr, "error loading fragment shader\n%s", log);
			loaded = false;
		}
	}

	if(loaded){
		id = glCreateProgram();
		glAttachShader(id, vertex_shader);
		glAttachShader(id, fragment_shader);
		glLinkProgram(id);

		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		int sucess;
		char log[512];
		glGetProgramiv(id, GL_LINK_STATUS, &sucess);

		if(!sucess){
			glGetProgramInfoLog(id, 512, NULL, log);
			fprintf(stderr, "error linking shader program\n%s", log);
		}
	}

	return loaded;
}

bool Shader::loadFile(const std::string& vertex_filename, const std::string& fragment_filename){
	std::string vertex_src, fragment_src;

	{
		std::ifstream file(vertex_filename);
		std::string line;

		if(!file.is_open()){
			fprintf(stderr, "Failed to open file: %s\n", vertex_filename.c_str());
			return false;
		}

		while(std::getline(file, line)){
			vertex_src += line + '\n';
		}
	}

	{
		std::ifstream file(fragment_filename);
		std::string line;

		if(!file.is_open()){
			fprintf(stderr, "Failed to open file: %s\n", fragment_filename.c_str());
			return false;
		}

		while(std::getline(file, line)){
			fragment_src += line + '\n';
		}
	}

	return loadShader(vertex_src.c_str(), fragment_src.c_str());
}

void Shader::use(void){
	if(loaded){
		glUseProgram(id);
	}
}

bool Shader::setUniform(const std::string& name, float x, float y, float z, float w){
	int location = getLocation(name);

	if(location < 0)
		return false;

	glUseProgram(id);
	glUniform4f(location, x, y, z, w);

	return true;
}

bool Shader::setUniform(const std::string& name, const Mat4& mat4){
	int location = getLocation(name);

	if(location < 0)
		return false;

	glUseProgram(id);
	glUniformMatrix4fv(location, 1, GL_TRUE, mat4.arr);

	return true;
}

bool Shader::setUniform(const std::string& name, int index){
	int location = getLocation(name);

	if(location < 0)
		return false;

	glUseProgram(id);
	glUniform1i(location, index);

	return true;
}

bool Shader::setUniform(const std::string& name, float index){
	int location = getLocation(name);

	if(location < 0)
		return false;

	glUseProgram(id);
	glUniform1f(location, index);

	return true;
}

void Shader::setTexture(const std::string& name, int texture_id, int index){
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, texture_id);

	setUniform(name, index);
}

Shader::~Shader(void){
	if(loaded)
		glDeleteProgram(id);
}

int Shader::getLocation(const std::string& name){
	return glGetUniformLocation(id, name.c_str());
}

};
