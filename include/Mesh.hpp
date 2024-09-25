#ifndef MESH_HPP
#define MESH_HPP

#include "Vec3.hpp"
#include <vector>
#include <array>
#include <cstdint>
#include <string>

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
	bool subdivideForUVNormal(std::vector<Triangle> *list);
};

struct Mesh {
	std::vector<Triangle> triangles;
	std::string texture_filename;

	Mesh(void);
	Mesh(std::string texture_filename);

	void addTriangle(Triangle triangle);
	void buildUnitTetrahedron(void);

	template <typename functon>
	Mesh applyTransformation(functon transform){
		Mesh mesh(texture_filename);

		for(auto& triangle : triangles){
			mesh.addTriangle(transform(triangles));
		}

		return mesh;
	}
};

};

#endif
