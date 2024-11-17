#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include "Mat4.hpp"

namespace acc {

class Shader {
	public:
		Shader(void);
		bool loadShader(const char *vertex_src, const char *fragment_shader);
		bool loadFile(const std::string& vertex_filename, const std::string& fragment_filename);
		void use(void);
		bool setUniform(const std::string& name, float x, float y, float z, float w);
		bool setUniform(const std::string& name, const Mat4& mat4);

		~Shader(void);

	private:
		int getLocation(const std::string& name);

		unsigned int id;
		bool loaded;
};

};

#endif
