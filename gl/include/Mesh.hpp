#ifndef MESH_HPP
#define MESH_HPP

#include "Vec3.hpp"
#include "Shader.hpp"

#include <vector>

namespace acc {

struct Vertex {
	float x, y, z, s, t;
};

class Mesh {
	public:
		Mesh(void);
		void load(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
		void render(Shader* shader);
		~Mesh(void);

	private:
		void setUp(void);

		unsigned int vao, vbo, ebo;
		bool loaded;

		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

};

};

#endif
