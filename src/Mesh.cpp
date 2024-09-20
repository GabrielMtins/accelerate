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

void Triangle::subdivideForUVNormal(std::vector<Triangle> *list){
	Triangle current;
	std::queue<Triangle> triangle_queue;

	triangle_queue.push(*this);

	while(!triangle_queue.empty()){
		current = triangle_queue.front();
		current.modOutUV();
		triangle_queue.pop();
		
		auto& vertices = current.vertices;

		current.sortByUVx();

		/* check if the length is greater than 1.0f, than
		 * subdivide in 4 if it is. Otherwise, check
		 * the limits of the vertices */

		/* subdivide it in the x axis */
		if(vertices[2].uv.x > 1.0f){
			float step = 1.0f - vertices[0].uv.x;
			float side = vertices[2].uv.x - vertices[0].uv.x;

			Vertex division_point = (vertices[2] - vertices[0]) * step / side + vertices[0];

			triangle_queue.push(Triangle(vertices[0], division_point, vertices[1]));
			triangle_queue.push(Triangle(vertices[2], division_point, vertices[1]));

			continue;
		}

		current.sortByUVy();

		if(vertices[2].uv.y > 1.0f){
			float step = 1.0f - vertices[0].uv.y;
			float side = vertices[2].uv.y - vertices[0].uv.y;

			Vertex division_point = (vertices[2] - vertices[0]) * step / side + vertices[0];

			triangle_queue.push(Triangle(vertices[0], division_point, vertices[1]));
			triangle_queue.push(Triangle(vertices[2], division_point, vertices[1]));

			continue;
		}

		list->push_back(current);
	}
}

void Mesh::addVertex(Vertex vertex){
	vertices.push_back(vertex);
}

void Mesh::addIndex(int index){
	indices.push_back(index);
}

std::vector<Vertex>& Mesh::getVertices(void){
	return vertices;
}

std::vector<int>& Mesh::getIndices(void){
	return indices;
}

};
