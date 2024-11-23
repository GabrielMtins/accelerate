#include "Mesh.hpp"

#include "glad/glad.h"
#include <cstddef>

namespace acc {

Vertex::Vertex(float x, float y, float z, float s, float t){
	position = Vec3(x, y, z);

	this->s = s;
	this->t = t;

	normal = Vec3(1.0f, 1.0f, 1.0f);
}

Vertex::Vertex(float x, float y, float z, float s, float t, float nx, float ny, float nz){
	position = Vec3(x, y, z);

	this->s = s;
	this->t = t;

	normal = Vec3(nx, ny, nz);
}

Mesh::Mesh(void){
	loaded = false;
}

Mesh::Mesh(const std::string& name){
	loaded = false;
	setName(name);
}

void Mesh::load(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices){
	loaded = true;

	this->vertices = vertices;
	this->indices = indices;

	setUp();
}

void Mesh::render(Shader* shader){
	if(!loaded)
		return;

	shader->use();

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::buildUnitQuad(void){
	loaded = true;

	vertices.emplace_back(-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	vertices.emplace_back(+0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	vertices.emplace_back(-0.5f, +0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	vertices.emplace_back(+0.5f, +0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f);

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	indices.push_back(3);
	indices.push_back(1);
	indices.push_back(2);

	setUp();
}

void Mesh::buildUnitTetrahedron(void){
	loaded = true;

	vertices.emplace_back(+0.5f, +0.0f, -0.35f, 0.0f, 0.0f);
	vertices.emplace_back(-0.5f, +0.0f, -0.35f, 0.0f, 1.0f);
	vertices.emplace_back(+0.0f, +0.5f, +0.35f, 1.0f, 0.0f);
	vertices.emplace_back(+0.0f, -0.5f, +0.35f, 1.0f, 1.0f);

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(3);

	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(3);
	
	indices.push_back(2);
	indices.push_back(0);
	indices.push_back(3);

	setUp();
}

void Mesh::buildUnitCube(void){
	loaded = true;

	vertices.emplace_back(+0.5f, -0.5f, -0.5f, 0.0f, 0.0f);
	vertices.emplace_back(+0.5f, -0.5f, +0.5f, 0.0f, 0.0f);
	vertices.emplace_back(-0.5f, -0.5f, +0.5f, 0.0f, 0.0f);
	vertices.emplace_back(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f);
	vertices.emplace_back(+0.5f, +0.5f, -0.5f, 0.0f, 0.0f);
	vertices.emplace_back(+0.5f, +0.5f, +0.5f, 0.0f, 0.0f);
	vertices.emplace_back(-0.5f, +0.5f, +0.5f, 0.0f, 0.0f);
	vertices.emplace_back(-0.5f, +0.5f, -0.5f, 0.0f, 0.0f);

	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(3);

	indices.push_back(7);
	indices.push_back(6);
	indices.push_back(5);

	indices.push_back(4);
	indices.push_back(5);
	indices.push_back(1);

	indices.push_back(5);
	indices.push_back(6);
	indices.push_back(2);

	indices.push_back(2);
	indices.push_back(6);
	indices.push_back(7);

	indices.push_back(0);
	indices.push_back(3);
	indices.push_back(7);

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(3);

	indices.push_back(4);
	indices.push_back(7);
	indices.push_back(5);

	indices.push_back(0);
	indices.push_back(4);
	indices.push_back(1);

	indices.push_back(1);
	indices.push_back(5);
	indices.push_back(2);

	indices.push_back(3);
	indices.push_back(2);
	indices.push_back(7);

	indices.push_back(4);
	indices.push_back(0);
	indices.push_back(7);

	setUp();
}

Mesh::~Mesh(void){
	if(loaded){
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ebo);
		glDeleteVertexArrays(1, &vao);
	}
}

void Mesh::computeNormals(void){
	Vec3 center;

	for(size_t i = 0; i < indices.size(); i += 3){
		Vec3 opposite;

		Vec3 a = vertices[indices[i]].position;
		Vec3 b = vertices[indices[i + 1]].position;
		Vec3 c = vertices[indices[i + 2]].position;

		for(size_t j = 0; j < vertices.size(); j++){
			if(j == i)
				continue;

			if(j == i + 1)
				continue;

			if(j == i + 2)
				continue;

			opposite = vertices[indices[j]].position;
		}

		Vec3 normal = Vec3::cross(a - b, a - c).normalize();

		if(Vec3::dot(normal, opposite - a) > 0.0f){
			normal *= -1.0f;
		}
		
		vertices[indices[i]].normal += normal;
		vertices[indices[i + 1]].normal += normal;
		vertices[indices[i + 2]].normal += normal;
	}

	for(auto i : indices){
		vertices[i].normal = vertices[i].normal.normalize() * (-1.0f);
	}
}

void Mesh::setUp(void){
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) (5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

};
