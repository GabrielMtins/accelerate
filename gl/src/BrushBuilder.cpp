#include "BrushBuilder.hpp"

namespace acc {

BrushBuilder::BrushBuilder(void){
}

Model3d * BrushBuilder::build(void){
	Model3d *model3d = new Model3d();

	model3d->addMesh(buildFlat(bottom_height, bottom_offset, bottom_scaling), bottom_texture);
	model3d->addMesh(buildFlat(top_height, top_offset, top_scaling), top_texture);
	model3d->addMesh(buildWall(), wall_texture);

	return model3d;
}

void BrushBuilder::setGeometry(const std::vector<Vec3>& vertices){
	this->vertices = vertices;
}

void BrushBuilder::setBottomProperties(TextureGL *bottom_texture, float bottom_height, Vec3 bottom_offset, Vec3 bottom_scaling){
	this->bottom_texture = bottom_texture;
	this->bottom_height = bottom_height;
	this->bottom_offset = bottom_offset;
	this->bottom_scaling = bottom_scaling;
}

void BrushBuilder::setTopProperties(TextureGL *top_texture, float top_height, Vec3 top_offset, Vec3 top_scaling){
	this->top_texture = top_texture;
	this->top_height = top_height;
	this->top_offset = top_offset;
	this->top_scaling = top_scaling;
}

void BrushBuilder::setWallProperties(TextureGL *wall_texture, Vec3 wall_offset, Vec3 wall_scaling){
	this->wall_texture = wall_texture;
	this->wall_offset = wall_offset;
	this->wall_scaling = wall_scaling;
}

Mesh * BrushBuilder::buildFlat(float height, const Vec3& offset, const Vec3& scale){
	std::vector<Vertex> conv_vertices;
	std::vector<unsigned int> indices;

	for(Vec3& i : vertices){
		conv_vertices.emplace_back(i.x, height, i.z, i.x, i.z);
	}

	for(Vertex& i : conv_vertices){
		i.s = i.s * scale.x + offset.x;
		i.t = i.t * scale.y + offset.y;
	}

	for(size_t i = 1; i < vertices.size() - 1; i++){
		indices.push_back(0);
		indices.push_back(i);
		indices.push_back(i + 1);
	}

	Mesh *mesh = new Mesh();

	mesh->load(conv_vertices, indices);

	return mesh;
}

Mesh * BrushBuilder::buildWall(void){
	std::vector<Vertex> conv_vertices;
	std::vector<unsigned int> indices;

	float accumulator = 0.0f;

	for(size_t i = 0; i < vertices.size() + 1; i++){
		size_t actual_index = i % vertices.size();
		Vec3& current = vertices[actual_index];

		conv_vertices.emplace_back(current.x, bottom_height, current.z, accumulator, 0.0f);
		accumulator += (vertices[(actual_index + 1) % vertices.size()] - vertices[actual_index]).length();
	}

	accumulator = 0.0f;

	for(size_t i = 0; i < vertices.size() + 1; i++){
		size_t actual_index = i % vertices.size();
		Vec3& current = vertices[actual_index];

		conv_vertices.emplace_back(current.x, top_height, current.z, accumulator, top_height - bottom_height);
		accumulator += (vertices[(actual_index + 1) % vertices.size()] - vertices[actual_index]).length();
	}

	for(Vertex& i : conv_vertices){
		i.s = i.s * wall_scaling.x + wall_offset.x;
		i.t = i.t * wall_scaling.y + wall_offset.y;
	}

	for(size_t i = 0; i < vertices.size(); i++){
		indices.push_back(i);
		indices.push_back(i + 1);
		indices.push_back(i + 1 + vertices.size() + 1);

		indices.push_back(i);
		indices.push_back(i + vertices.size() + 1);
		indices.push_back(i + 1 + vertices.size() + 1);
	}

	Mesh *mesh = new Mesh();

	mesh->load(conv_vertices, indices);

	return mesh;
}

};
