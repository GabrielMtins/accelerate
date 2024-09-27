#ifndef MESH_HPP
#define MESH_HPP

#include "Vec3.hpp"
#include <vector>
#include <array>
#include <algorithm>
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
	/* the function "apply" will receive this arguments:
	 * (int i, int j, Vec3 uv, Vec3 color, float z);
	 */
	template <typename functon>
	void rasterize(functon apply){
		sortByY();

		for(auto& vertex : vertices){
			vertex.uv.z = 1.0f;
			vertex.uv /= vertex.position.z;
			vertex.color /= vertex.position.z;
		}

		int start_y = vertices[0].position.y;
		int middle_y = vertices[1].position.y;
		int end_y = vertices[2].position.y;

		for(int j = start_y; j < middle_y; j++){
			Vertex a = vertices[0] + (vertices[2] - vertices[0]) * (j - start_y) / (end_y - start_y);
			Vertex b = vertices[0] + (vertices[1] - vertices[0]) * (j - start_y) / (middle_y - start_y);

			if(a.position.x > b.position.x)
				std::swap(a, b);

			for(int i = a.position.x; i < b.position.x; i++){
				Vertex c = a + (b - a) * (i - a.position.x) / (b.position.x - a.position.x);
				float z = 1.0f / c.uv.z;

				apply(i, j, c.uv * z, c.color * z, z);
			}
		}

		Vertex middle_vertex = vertices[0] + (vertices[2] - vertices[0]) * (middle_y - start_y) / (end_y - start_y);

		for(int j = middle_y; j < end_y; j++){
			Vertex a = vertices[1] + (vertices[2] - vertices[1]) * (j - middle_y) / (end_y - middle_y);
			Vertex b = middle_vertex + (vertices[2] - middle_vertex) * (j - middle_y) / (end_y - middle_y);

			if(a.position.x > b.position.x)
				std::swap(a, b);

			for(int i = a.position.x; i < b.position.x; i++){
				Vertex c = a + (b - a) * (i - a.position.x) / (b.position.x - a.position.x);
				float z = 1.0f / c.uv.z;

				apply(i, j, c.uv * z, c.color * z, z);
			}
		}
	}

	void sortByY(void);
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

	/* the function "apply" will receive this arguments:
	 * (int i, int j, Vec3 uv, Vec3 color, float z);
	 */
	template <typename functon>
	void rasterize(functon apply){
		for(auto triangle : triangles){
			triangle.rasterize(apply);
		}
	}

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
