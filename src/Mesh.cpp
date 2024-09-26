#include "Mesh.hpp"

#include <cmath>
#include <algorithm>
#include <queue>

namespace acc {

Vertex::Vertex(void){
	this->position = Vec3();
	this->uv = Vec3();
	this->color = Vec3();
	this->alpha = 1.0f;
}

Vertex::Vertex(Vec3 position, Vec3 uv, Vec3 color, float alpha){
	this->position = position;
	this->uv = uv;
	this->color = color;
	this->alpha = alpha;
}

Vertex::Vertex(Vec3 position, Vec3 color, float alpha){
	this->position = position;
	this->color = color;
	this->alpha = alpha;
}

Vertex::Vertex(Vec3 position, Vec3 uv){
	this->position = position;
	this->uv = uv;
	this->color = Vec3(1.0f, 1.0f, 1.0f);
	this->alpha = 1.0f;
}

Vertex Vertex::operator+(const Vertex& obj){
	return Vertex(
			position + obj.position,
			uv + obj.uv,
			color + obj.color,
			alpha + obj.alpha
			);
}

Vertex Vertex::operator-(const Vertex& obj){
	return Vertex(
			position - obj.position,
			uv - obj.uv,
			color - obj.color,
			alpha - obj.alpha
			);
}

Vertex Vertex::operator*(const float& r){
	return Vertex(
			position * r,
			uv * r,
			color * r,
			alpha * r
			);
}

Vertex Vertex::operator/(const float& r){
	return Vertex(
			position / r,
			uv / r,
			color / r,
			alpha / r
			);
}

Triangle::Triangle(void){
	for(auto& i : vertices)
		i = Vertex();
}

Triangle::Triangle(const Vertex& a, const Vertex& b, const Vertex& c){
	vertices[0] = a;
	vertices[1] = b;
	vertices[2] = c;
}

Vec3 Triangle::getNormal(void){
	return Vec3::cross(
			vertices[2].position - vertices[1].position,
			vertices[2].position - vertices[0].position
			);
}

void Triangle::sortByUVx(void){
	std::sort(vertices.begin(), vertices.end(), [](Vertex a, Vertex b){return a.uv.x < b.uv.x;});
}

void Triangle::sortByUVy(void){
	std::sort(vertices.begin(), vertices.end(), [](Vertex a, Vertex b){return a.uv.y < b.uv.y;});
}

void Triangle::modOutUV(void){
	float min_value_x = 99999.0f;
	float min_value_y = 99999.0f;

	for(auto& i : vertices){
		if(i.uv.x < min_value_x) min_value_x = i.uv.x;
		if(i.uv.y < min_value_y) min_value_y = i.uv.y;
	}

	min_value_x = floorf(min_value_x);
	min_value_y = floorf(min_value_y);

	for(auto& i : vertices){
		i.uv.x -= min_value_x;
		i.uv.y -= min_value_y;
	}
}

float Triangle::maxUVLengthSqr(void){
	return
		fmaxf(
				(vertices[1].uv - vertices[0].uv).lengthSqr(),
				fmax(
					(vertices[2].uv - vertices[1].uv).lengthSqr(),
					(vertices[2].uv - vertices[0].uv).lengthSqr()
					)
			 );
}

bool Triangle::clipOverZ(std::vector<Triangle> *list){
	Triangle copy = *this;
	float clip_plane = 0.1f;

	std::sort(copy.vertices.begin(), copy.vertices.end(), 
			[](Vertex a, Vertex b){
				return a.position.z < b.position.z;
			}
			);

	/* it was clipped */
	if(copy.vertices[2].position.z < 0) return true;
	
	if(copy.vertices[1].position.z < 0){
		Vertex m12, m02;

		float step12 = (clip_plane - copy.vertices[1].position.z);
		float side12 = (copy.vertices[2].position.z - copy.vertices[1].position.z);

		m12 = (copy.vertices[2] - copy.vertices[1]) * step12 / side12 + copy.vertices[1];

		float step02 = (clip_plane - copy.vertices[0].position.z);
		float side02 = (copy.vertices[2].position.z - copy.vertices[0].position.z);

		m02 = (copy.vertices[2] - copy.vertices[0]) * step02 / side02 + copy.vertices[0];

		list->push_back(Triangle(m02, m12, copy.vertices[2]));
	
		return true;
	}

	if(copy.vertices[0].position.z < 0){
		Vertex m01, m02;

		float step = (clip_plane - copy.vertices[0].position.z);
		float side01 = (copy.vertices[1].position.z - copy.vertices[0].position.z);
		float side02 = (copy.vertices[2].position.z - copy.vertices[0].position.z);

		m01 = (copy.vertices[1] - copy.vertices[0]) * step / side01 + copy.vertices[0];

		m02 = (copy.vertices[2] - copy.vertices[0]) * step / side02 + copy.vertices[0];

		list->push_back(Triangle(m01, m02, copy.vertices[2]));
		list->push_back(Triangle(m01, copy.vertices[1], copy.vertices[2]));
	
		return true;
	}

	return false;
}

bool Triangle::subdivideForUVNormal(std::vector<Triangle> *list){
	Triangle current;
	std::queue<Triangle> triangle_queue;

	size_t original_size = list->size();

	triangle_queue.push(*this);

	while(!triangle_queue.empty()){
		current = triangle_queue.front();
		current.modOutUV();
		triangle_queue.pop();
		
		auto& vertices = current.vertices;

		/* check if the length is greater than 1.0f, than
		 * subdivide in 4 if it is. Otherwise, check
		 * the limits of the vertices */


		current.sortByUVy();

		if(vertices[2].uv.y > 1.0f){
			float step = 1.0f - vertices[0].uv.y;
			float side = vertices[2].uv.y - vertices[0].uv.y;

			Vertex division_point = (vertices[2] - vertices[0]) * step / side + vertices[0];

			triangle_queue.push(Triangle(vertices[0], division_point, vertices[1]));
			triangle_queue.push(Triangle(vertices[2], division_point, vertices[1]));

			continue;
		}

		current.sortByUVx();

		/* subdivide it in the x axis */
		if(vertices[2].uv.x > 1.0f){
			float step = 1.0f - vertices[0].uv.x;
			float side = vertices[2].uv.x - vertices[0].uv.x;

			Vertex division_point = (vertices[2] - vertices[0]) * step / side + vertices[0];

			triangle_queue.push(Triangle(vertices[0], division_point, vertices[1]));
			triangle_queue.push(Triangle(vertices[2], division_point, vertices[1]));

			continue;
		}

		list->push_back(current);
	}

	return original_size != list->size();
}

bool Triangle::compareLess(Triangle a, Triangle b){
	return Triangle::compareGreater(a, b);
}

bool Triangle::compareGreater(Triangle a, Triangle b){
	std::sort(a.vertices.begin(), a.vertices.end(), [](Vertex a, Vertex b){return a.position.z < b.position.z;});
	std::sort(b.vertices.begin(), b.vertices.end(), [](Vertex a, Vertex b){return a.position.z < b.position.z;});

	if(a.vertices[2].position.z < b.vertices[0].position.z){
		return false;
	}

	if(a.vertices[0].position.z > b.vertices[2].position.z){
		return true;
	}

	Vertex center_a = (a.vertices[0] + a.vertices[1] + a.vertices[2]) / 3;
	Vertex center_b = (b.vertices[0] + b.vertices[1] + b.vertices[2]) / 3;
	return center_a.position.lengthSqr() > center_b.position.lengthSqr();
}

Mesh::Mesh(void){
	this->texture_filename = "dev_texture_xor";
}

Mesh::Mesh(std::string texture_filename){
	this->texture_filename = texture_filename;
}

void Mesh::addTriangle(Triangle triangle){
	triangles.push_back(triangle);
}

void Mesh::buildUnitTetrahedron(void){
	Vec3 a, b, c, d;

	a = Vec3(+0.5f, 0.0f, -0.35f);
	b = Vec3(-0.5f, 0.0f, -0.35f);
	c = Vec3(0.0f, 0.5f, 0.35f);
	d = Vec3(0.0f, -0.5f, 0.35f);

	addTriangle(
			Triangle(
				Vertex(a, Vec3(0.0f, 0.0f)),
				Vertex(b, Vec3(1.0f, 0.0f)),
				Vertex(c, Vec3(0.0f, 1.0f))
				)
			);

	addTriangle(
			Triangle(
				Vertex(a, Vec3(0.0f, 0.0f)),
				Vertex(b, Vec3(1.0f, 0.0f)),
				Vertex(d, Vec3(0.0f, 1.0f))
				)
			);

	addTriangle(
			Triangle(
				Vertex(b, Vec3(0.0f, 0.0f)),
				Vertex(c, Vec3(1.0f, 0.0f)),
				Vertex(d, Vec3(0.0f, 1.0f))
				)
			);

	addTriangle(
			Triangle(
				Vertex(a, Vec3(0.0f, 0.0f)),
				Vertex(c, Vec3(1.0f, 0.0f)),
				Vertex(d, Vec3(0.0f, 1.0f))
				)
			);
}

void Mesh::buildUnitCube(void){
	addTriangle(
			Triangle(
				Vertex(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f)),
				Vertex(Vec3(1.0f, 0.0f, 0.0f), Vec3(1.0f, 0.0f)),
				Vertex(Vec3(0.0f, 1.0f, 0.0f), Vec3(0.0f, 1.0f))
				)
			);

	addTriangle(
			Triangle(
				Vertex(Vec3(1.0f, 1.0f, 0.0f), Vec3(1.0f, 1.0f)),
				Vertex(Vec3(1.0f, 0.0f, 0.0f), Vec3(1.0f, 0.0f)),
				Vertex(Vec3(0.0f, 1.0f, 0.0f), Vec3(0.0f, 1.0f))
				)
			);

	addTriangle(
			Triangle(
				Vertex(Vec3(0.0f, 0.0f, 1.0f), Vec3(0.0f, 0.0f)),
				Vertex(Vec3(1.0f, 0.0f, 1.0f), Vec3(1.0f, 0.0f)),
				Vertex(Vec3(0.0f, 1.0f, 1.0f), Vec3(0.0f, 1.0f))
				)
			);

	addTriangle(
			Triangle(
				Vertex(Vec3(1.0f, 1.0f, 1.0f), Vec3(1.0f, 1.0f)),
				Vertex(Vec3(1.0f, 0.0f, 1.0f), Vec3(1.0f, 0.0f)),
				Vertex(Vec3(0.0f, 1.0f, 1.0f), Vec3(0.0f, 1.0f))
				)
			);

	addTriangle(
			Triangle(
				Vertex(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f)),
				Vertex(Vec3(1.0f, 0.0f, 0.0f), Vec3(1.0f, 0.0f)),
				Vertex(Vec3(0.0f, 0.0f, 1.0f), Vec3(0.0f, 1.0f))
				)
			);

	addTriangle(
			Triangle(
				Vertex(Vec3(1.0f, 0.0f, 1.0f), Vec3(1.0f, 1.0f)),
				Vertex(Vec3(1.0f, 0.0f, 0.0f), Vec3(1.0f, 0.0f)),
				Vertex(Vec3(0.0f, 0.0f, 1.0f), Vec3(0.0f, 1.0f))
				)
			);

	addTriangle(
			Triangle(
				Vertex(Vec3(0.0f, 1.0f, 0.0f), Vec3(0.0f, 0.0f)),
				Vertex(Vec3(1.0f, 1.0f, 0.0f), Vec3(1.0f, 0.0f)),
				Vertex(Vec3(0.0f, 1.0f, 1.0f), Vec3(0.0f, 1.0f))
				)
			);

	addTriangle(
			Triangle(
				Vertex(Vec3(1.0f, 1.0f, 1.0f), Vec3(1.0f, 1.0f)),
				Vertex(Vec3(1.0f, 1.0f, 0.0f), Vec3(1.0f, 0.0f)),
				Vertex(Vec3(0.0f, 1.0f, 1.0f), Vec3(0.0f, 1.0f))
				)
			);

	addTriangle(
			Triangle(
				Vertex(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f)),
				Vertex(Vec3(0.0f, 0.0f, 1.0f), Vec3(1.0f, 0.0f)),
				Vertex(Vec3(0.0f, 1.0f, 0.0f), Vec3(0.0f, 1.0f))
				)
			);

	addTriangle(
			Triangle(
				Vertex(Vec3(0.0f, 1.0f, 1.0f), Vec3(0.0f, 0.0f)),
				Vertex(Vec3(0.0f, 0.0f, 1.0f), Vec3(1.0f, 0.0f)),
				Vertex(Vec3(0.0f, 1.0f, 0.0f), Vec3(0.0f, 1.0f))
				)
			);

	addTriangle(
			Triangle(
				Vertex(Vec3(1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f)),
				Vertex(Vec3(1.0f, 0.0f, 1.0f), Vec3(1.0f, 0.0f)),
				Vertex(Vec3(1.0f, 1.0f, 0.0f), Vec3(0.0f, 1.0f))
				)
			);

	addTriangle(
			Triangle(
				Vertex(Vec3(1.0f, 1.0f, 1.0f), Vec3(0.0f, 0.0f)),
				Vertex(Vec3(1.0f, 0.0f, 1.0f), Vec3(1.0f, 0.0f)),
				Vertex(Vec3(1.0f, 1.0f, 0.0f), Vec3(0.0f, 1.0f))
				)
			);

	applyTransformation([](Vertex a){ a.position -= Vec3(0.5f, 0.5f, 0.5f); return a; });
}

void Mesh::subdivide(void){
	std::vector<Triangle> tmp_vector = triangles;
	triangles = std::vector<Triangle>();
	triangles.reserve(4 * tmp_vector.size());

	while(tmp_vector.size() != 0){
		Triangle& triangle = tmp_vector.back();

		Vertex med01 = (triangle.vertices[0] + triangle.vertices[1]) / 2;
		Vertex med02 = (triangle.vertices[0] + triangle.vertices[2]) / 2;
		Vertex med12 = (triangle.vertices[1] + triangle.vertices[2]) / 2;

		triangles.push_back(
				Triangle(
					triangle.vertices[0],
					med01,
					med02
					)
				);

		triangles.push_back(
				Triangle(
					triangle.vertices[1],
					med01,
					med12
					)
				);

		triangles.push_back(
				Triangle(
					triangle.vertices[2],
					med02,
					med12
					)
				);

		triangles.push_back(
				Triangle(
					med01,
					med02,
					med12
					)
				);

		tmp_vector.pop_back();
	}
}

void Mesh::clipDepth(void){
	std::vector<Triangle> tmp_vector = triangles;
	triangles = std::vector<Triangle>();
	triangles.reserve(tmp_vector.size());

	while(tmp_vector.size() != 0){
		if(!tmp_vector.back().clipOverZ(&triangles)){
			triangles.push_back(tmp_vector.back());
		}

		tmp_vector.pop_back();
	}
}

void Mesh::clipUV(void){
	std::vector<Triangle> tmp_vector = triangles;
	triangles = std::vector<Triangle>();
	triangles.reserve(tmp_vector.size());

	while(tmp_vector.size() != 0){
		tmp_vector.back().subdivideForUVNormal(&triangles);

		tmp_vector.pop_back();
	}
}

void Mesh::projectOrtographic(int width, int height){
	applyTransformation([width, height](Vertex vertex){
		vertex.position.x *= height / 2;
		vertex.position.y *= height / 2;

		vertex.position.x += width / 2;
		vertex.position.y += height / 2;

		return vertex;
	}
	);
}

void Mesh::projectPerspective(int width, int height){
	applyTransformation([width, height](Vertex vertex){
		vertex.position.x /= vertex.position.z;
		vertex.position.y /= vertex.position.z;
		vertex.position.x *= height / 2;
		vertex.position.y *= height / 2;

		vertex.position.x += width / 2;
		vertex.position.y += height / 2;

		return vertex;
	}
	);
}

void Mesh::sortByDepth(void){
	std::sort(triangles.begin(), triangles.end(), Triangle::compareGreater);
}

void Mesh::applyLight(Vec3 bg, float max_distance){
	Vec3 direction = Vec3(0.0f, 0.3f, 1.0f).normalize();

	for(auto& triangle : triangles){
		Vec3 normal = triangle.getNormal().normalize();

		float normal_perc = fabsf(Vec3::dot(normal, direction)) * 0.4f + 0.6f;

		for(auto& i : triangle.vertices){
			float depth_perc = 1.0f - (i.position.lengthSqr()) / (max_distance * max_distance);

			if(depth_perc < 0.0f) depth_perc = 0.0f;
			if(depth_perc > 1.0f) depth_perc = 1.0f;

			float perc = depth_perc * normal_perc;

			i.color = i.color * perc + bg * (1.0f - perc);
		}
	}
}

};
