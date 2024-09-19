#ifndef MESH_HPP
#define MESH_HPP

#include "Vec3.hpp"
#include <vector>
#include <cstdint>

namespace acc {

struct Vertex {
	Vec3 position;
	Vec3 uv;
	Vec3 color;
	uint8_t alpha;

	Vertex(Vec3 position, Vec3 uv, Vec3 color, uint8_t alpha);
	Vertex(Vec3 position, Vec3 color, uint8_t alpha);
	Vertex(Vec3 position, Vec3 uv);
};

class Mesh {
	public:
		void addVertex(Vertex vertex);
		void addIndex(int index);

		std::vector<Vertex>& getVertices(void);
		std::vector<int>& getIndices(void);

		template <typename functon>
		void applyTransformation(functon transform){
			/*
			for(size_t i = 0; i < vertices.size(); i++){
				vertices[i] = transform(vertices[i]);
			}
			*/
			for(auto& vertex : vertices){
				vertex = transform(vertex);
			}
		}
	
	private:
		std::vector<Vertex> vertices;
		std::vector<int> indices;
};

};

#endif
