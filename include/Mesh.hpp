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

	Vec3 getNormal(void);
	void sortByUVx(void);
	void sortByUVy(void);
	void modOutUV(void);
	float maxUVLengthSqr(void);

	bool clipOverZ(std::vector<Triangle> *list);
	bool subdivideForUVNormal(std::vector<Triangle> *list);

	static bool compareLess(Triangle a, Triangle b);
	static bool compareGreater(Triangle a, Triangle b);
};

struct Mesh {
	std::vector<Triangle> triangles;
	std::string texture_filename;

	Mesh(void);
	Mesh(std::string texture_filename);

	void addTriangle(Triangle triangle);
	void buildUnitTetrahedron(void);
	void buildUnitCube(void);
	void subdivide(void);
	void clipDepth(void);
	void clipUV(void);
	void projectOrtographic(int width, int height);
	void projectPerspective(int width, int height);
	void sortByDepth(void);
	void applyLight(Vec3 bg, float max_distance);

	template <typename functon>
	void applyTransformation(functon transform){
		for(auto& triangle : triangles){
			for(auto& i : triangle.vertices){
				i = transform(i);
			}
		}
	}
};

};

#endif
