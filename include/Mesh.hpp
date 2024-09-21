#ifndef MESH_HPP
#define MESH_HPP

#include "Vec3.hpp"
#include <vector>
#include <array>
#include <cstdint>

namespace acc {

struct Vertex {
	Vec3 position;
	Vec3 uv;
	Vec3 color;
	float alpha;

	Vertex(void);
	Vertex(Vec3 position, Vec3 uv, Vec3 color, float alpha);
	Vertex(Vec3 position, Vec3 color, float alpha);
	Vertex(Vec3 position, Vec3 uv);

	Vertex operator+(const Vertex& obj);
	Vertex operator-(const Vertex& obj);
	Vertex operator*(const float& r);
	Vertex operator/(const float& r);
};

struct Triangle {
	std::array<Vertex, 3> vertices;

	Triangle(void);
	Triangle(const Vertex& a, const Vertex& b, const Vertex& c);

	template <typename functon>
	void applyTransformation(functon transform){
		for(auto& vertex : vertices){
			vertex = transform(vertex);
		}
	}

	void sortByUVx(void);
	void sortByUVy(void);
	void modOutUV(void);
	float maxUVLengthSqr(void);

	bool clipOverZ(std::vector<Triangle> *list);
	void subdivideForUVNormal(std::vector<Triangle> *list);
};

class Mesh {
	public:
		void addVertex(Vertex vertex);
		void addIndex(int index);

		std::vector<Vertex>& getVertices(void);
		std::vector<int>& getIndices(void);

		template <typename functon>
		void applyTransformation(functon transform){
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
