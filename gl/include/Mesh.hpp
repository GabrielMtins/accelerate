#ifndef MESH_HPP
#define MESH_HPP

#include "Vec3.hpp"
#include "Shader.hpp"
#include "Resource.hpp"

#include <vector>

namespace acc {

struct Vertex {
	Vec3 position;
	float s, t;
	Vec3 normal;

	Vertex(float x, float y, float z, float s, float t);
	Vertex(float x, float y, float z, float s, float t, float nx, float ny, float nz);
};

class Mesh : public Resource {
	public:
		Mesh(void);
		Mesh(const std::string& name);
		void buildUnitQuad(void);
		void buildUnitTetrahedron(void);
		void buildUnitCube(void);
		void load(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
		void render(Shader* shader);
		~Mesh(void);

		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

	private:
		void computeNormals(void);
		void setUp(void);

		unsigned int vao, vbo, ebo;
		bool loaded;


};

};

#endif
