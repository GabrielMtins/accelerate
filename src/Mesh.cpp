#include "Mesh.hpp"

namespace acc {

Vertex::Vertex(Vec3 position, Vec3 uv, Vec3 color, uint8_t alpha){
	this->position = position;
	this->uv = uv;
	this->color = color;
	this->alpha = alpha;
}

Vertex::Vertex(Vec3 position, Vec3 uv){
	this->position = position;
	this->uv = uv;
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
