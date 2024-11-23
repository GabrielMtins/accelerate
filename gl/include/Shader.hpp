#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include "Mat4.hpp"
#include "Resource.hpp"

namespace acc {

class Shader : public Resource{
	public:
		Shader(void);
		Shader(const std::string& name);
		Shader(const std::string &name, const std::string &vertex_filename, const std::string& fragment_filename);
		bool loadShader(const char *vertex_src, const char *fragment_shader);
		bool loadFile(const std::string& vertex_filename, const std::string& fragment_filename);
		void use(void);
		bool setUniform(const std::string& name, float x, float y, float z, float w);
		bool setUniform(const std::string& name, const Mat4& mat4);
		bool setUniform(const std::string& name, int index);
		bool setUniform(const std::string& name, float index);
		void setTexture(const std::string& name, int texture_id, int index);

		~Shader(void);

	private:
		int getLocation(const std::string& name);

		unsigned int id;
		bool loaded;
};

};

#endif
