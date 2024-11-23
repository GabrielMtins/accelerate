#include "BrushBuilder.hpp"

namespace acc {

BrushBuilder::BrushBuilder(void){
	model3d = NULL;
	bottom_ramp_wall_index = top_ramp_wall_index = -1;
}

Model3d * BrushBuilder::build(void){
	if(model3d != NULL){
		delete model3d;
		model3d = NULL;
	}

	computeNormals();
	model3d = new Model3d();

	model3d->addMesh(buildFlat(bottom_ramp_wall_index, bottom_inclination, bottom_height, bottom_offset, bottom_scaling), bottom_texture);
	model3d->addMesh(buildFlat(top_ramp_wall_index, top_inclination, top_height, top_offset, top_scaling), top_texture);
	model3d->addMesh(buildWall(), wall_texture);

	buildBoundingCylinder();

	return model3d;
}

void BrushBuilder::reset(void){
	model3d = NULL;
}

Model3d * BrushBuilder::getModel(void){
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

void BrushBuilder::setTopRamp(float angle, int wall_index){
	top_inclination = angle;
	top_ramp_wall_index = wall_index;
}

void BrushBuilder::setBottomRamp(float angle, int wall_index){
	bottom_inclination = angle;
	bottom_ramp_wall_index = wall_index;
}

bool BrushBuilder::checkCollisionCylinder(Vec3 position, float radius, float height, float tolerance, Vec3 *delta){
	if(!checkBoundingCylinder(position, radius, height))
		return false;

	float disp_y = 0.0f;

	float actual_bottom_height = getHeight(bottom_ramp_wall_index, bottom_height, bottom_inclination, position);
	float actual_top_height = getHeight(top_ramp_wall_index, top_height, top_inclination, position);

	if(position.y + height <= actual_bottom_height){
		return false;
	}
	else{
		disp_y = -(position.y + height - actual_bottom_height);
	}

	if(position.y > actual_top_height){
		return false;
	}
	else{
		if(fabsf(actual_top_height - position.y) < fabsf(disp_y))
			disp_y = (actual_top_height - position.y);
	}

	if(checkCollisionPoint(position)){
		if(delta != NULL)
			*delta = Vec3(0.0f, disp_y, 0.0f);

		return true;
	}

	if(checkCollisionCircle(position, radius, delta)){
		if(delta != NULL){
			if(disp_y * disp_y < delta->lengthSqr()){
				*delta = Vec3(0.0f, disp_y, 0.0f);
			}
		}

		return true;
	}

	return false;
}

Vec3 BrushBuilder::normal2D(Vec3 t){
	return Vec3(t.z, 0.0f, -t.x);
}

void BrushBuilder::computeNormals(void){
	normals.clear();

	for(size_t i = 0; i < vertices.size(); i++){
		Vec3 a = vertices[i];
		Vec3 b = vertices[(i + 1) % vertices.size()];
		Vec3 c = vertices[(i + 2) % vertices.size()];

		Vec3 normal = normal2D(a - b);

		if(Vec3::dot(c - a, normal) > 0.0f)
			normal *= -1.0f;

		normals.push_back(normal.normalize());
	}
}

bool BrushBuilder::checkCollisionPoint(Vec3 point){
	point.y = 0.0f;
	Vec3 a = vertices[0];
	a.y = 0.0f;

	for(size_t i = 1; i < vertices.size() - 1; i++){
		Vec3 b = vertices[i];
		Vec3 c = vertices[i + 1];

		Vec3 normal_ab = normal2D(a - b);
		if(Vec3::dot(normal_ab, (c - a)) > 0.0f){
			normal_ab *= -1.0f;
		}
		if(Vec3::dot(point - a, normal_ab) > 0.0f)
			continue;

		Vec3 normal_ac = normal2D(c - a);
		if(Vec3::dot(normal_ac, (b - a)) > 0.0f){
			normal_ac *= -1.0f;
		}
		if(Vec3::dot(point - a, normal_ac) > 0.0f)
			continue;

		Vec3 normal_bc = normal2D(b - c);
		if(Vec3::dot(normal_bc, (a - b)) > 0.0f){
			normal_bc *= -1.0f;
		}
		if(Vec3::dot(point - b, normal_bc) > 0.0f)
			continue;

		return true;
	}

	return false;
}

bool BrushBuilder::checkCollisionCircle(Vec3 position, float radius, Vec3 *delta){
	position.y = 0.0f;

	for(size_t j = 0; j < vertices.size(); j++){
		Vec3 now = vertices[j];
		Vec3 next = vertices[(j + 1) % vertices.size()];
		now.y = next.y = 0.0f;

		Vec3 normal = normals[j];
		float distance = Vec3::dot(position - now, normal);

		/*
		if(j == 0)
			normal.print();
			*/

		if(fabsf(distance) > radius)
			continue;

		Vec3 middle = position - normal * fabsf(distance);

		if(Vec3::dot(now - middle, next - middle) < 0.0f){
			if(delta != NULL){
				*delta = normal * (radius - distance);
			}

			return true;
		}
	}

	for(const Vec3& vertex : vertices){
		Vec3 dir = (position - vertex);

		if(dir.lengthSqr() > radius * radius)
			continue;

		if(delta != NULL){
			float length = dir.length();
			*delta = dir * ((radius - length) / length);
		}

	  return true;
	}

	return false;
}

bool BrushBuilder::checkCollisionLine(Vec3 start, Vec3 end, Vec3 *intersection){
	end.y = 0.0f;
	start.y = 0.0f;

	Vec3 normal = (end - start);
	normal = Vec3(normal.z, 0.0f, -normal.x);

	for(size_t j = 0; j < vertices.size(); j++){
		Vec3 now = vertices[j];
		Vec3 next = vertices[(j + 1) % vertices.size()];

		if(Vec3::dot(normal, next - now) == 0.0f)
			continue;

		float u = (Vec3::dot(start, normal) - Vec3::dot(normal, now)) / (Vec3::dot(normal, next - now));

		if(u < 0.0f || u > 1.0f)
			continue;

		Vec3 vertex = now + (next - now) * u;

		if(vertex.x >= start.x && vertex.x <= end.x && vertex.z >= start.z && vertex.z <= end.z){
			if(intersection != NULL)
				*intersection = vertex;

			return true;
		}

	}

	return false;
}

float BrushBuilder::getHeight(int ramp_wall_index, float height, float inclination, const Vec3& position){
	if(ramp_wall_index < 0)
		return height;

	Vec3 now, next;

	now = vertices[ramp_wall_index];
	next = vertices[(ramp_wall_index + 1) % vertices.size()];
	Vec3 normal = normal2D(now - next).normalize();

	float distance = fabsf(Vec3::dot(position - now, normal));

	return height + distance * (inclination);
}

Mesh * BrushBuilder::buildFlat(int ramp_wall_index, float inclination, float height, const Vec3& offset, const Vec3& scale){
	std::vector<Vertex> conv_vertices;
	std::vector<unsigned int> indices;


	for(size_t i = 0; i < vertices.size(); i++){
		conv_vertices.emplace_back(
				vertices[i].x,
				getHeight(ramp_wall_index, height, inclination, vertices[i]),
				vertices[i].z,
				vertices[i].x,
				vertices[i].z
				);
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

		float new_bottom_height = getHeight(bottom_ramp_wall_index, bottom_height, bottom_inclination, current);

		conv_vertices.emplace_back(
				current.x,
				new_bottom_height,
				current.z,
				accumulator,
				new_bottom_height - bottom_height
				);

		accumulator += (vertices[(actual_index + 1) % vertices.size()] - vertices[actual_index]).length();
	}

	accumulator = 0.0f;

	for(size_t i = 0; i < vertices.size() + 1; i++){
		size_t actual_index = i % vertices.size();
		Vec3& current = vertices[actual_index];
		
		float new_top_height = getHeight(top_ramp_wall_index, top_height, top_inclination, current);

		conv_vertices.emplace_back(
				current.x,
				new_top_height,
				current.z,
				accumulator,
				new_top_height - bottom_height
				);

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

bool BrushBuilder::checkBoundingCylinder(Vec3 position, float radius, float height){
	if(position.y > max_height)
		return false;

	if(position.y + height < min_height)
		return false;

	position.y = 0.0f;

	float dist_sqr = (position - center).lengthSqr();
	float max_dist_sqr = radius + this->radius;
	max_dist_sqr *= max_dist_sqr;

	if(dist_sqr > max_dist_sqr)
		return false;

	return true;
}

void BrushBuilder::buildBoundingCylinder(void){
	float min_x = vertices[0].x, min_z = vertices[0].z, max_x = vertices[0].x, max_z = vertices[0].z;
	min_height = top_height, max_height = bottom_height;

	for(const Vec3& vertex : vertices){
		min_x = fminf(min_x, vertex.x);
		min_z = fminf(min_z, vertex.z);

		max_x = fmaxf(max_x, vertex.x);
		max_z = fmaxf(max_z, vertex.z);

		float actual_bottom_height = getHeight(bottom_ramp_wall_index, bottom_height, bottom_inclination, vertex);
		float actual_top_height = getHeight(top_ramp_wall_index, top_height, top_inclination, vertex);

		min_height = fminf(min_height, actual_bottom_height);
		min_height = fminf(min_height, actual_top_height);

		max_height = fmaxf(max_height, actual_bottom_height);
		max_height = fmaxf(max_height, actual_top_height);
	}

	radius = sqrt((min_x - max_x) * (min_x - max_x) + (min_z - max_z) * (min_z - max_z)) / 2;
	center = Vec3(min_x + max_x, 0.0f, min_z + max_z) * 0.5f;
}

};
