#ifndef MESH_HPP
#define MESH_HPP

#include "Vec3.hpp"
#include "Shader.hpp"
#include "Resource.hpp"

#include <vector>

namespace acc {

struct Vertex {
	float x, y, z, s, t;
	float nx, ny, nz;

	Vertex(float x, float y, float z, float s, float t);
};

class Mesh : public Resource {
	public:
		Mesh(void);
		Mesh(const std::string& name);
		void buildUnitQuad(void);
		void buildUnitTetrahedron(void);
		void load(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
		void render(Shader* shader);
		~Mesh(void);

		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

	private:
		void setUp(void);

		unsigned int vao, vbo, ebo;
		bool loaded;


};

};

#endif
