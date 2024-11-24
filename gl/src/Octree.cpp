#include "Octree.hpp"
#include "Components/Body3dComponent.hpp"

#include <unordered_set>
#include <stack>

namespace acc {

OctreeNode::OctreeNode(const Vec3& start, const Vec3& size, size_t depth, size_t max_elements){
	this->start = start;
	this->size = size;
	this->depth = depth;
	child = -1;
	stop_subdivide = false;

	brushes.reserve(max_elements);
}

bool OctreeNode::checkCollision(const Vec3& other_start, const Vec3& other_size){
	if(other_start.x + other_size.x <= start.x)
		return false;

	if(other_start.y + other_size.y <= start.y)
		return false;

	if(other_start.z + other_size.z <= start.z)
		return false;

	if(other_start.x >= start.x + size.x)
		return false;

	if(other_start.y >= start.y + size.y)
		return false;

	if(other_start.z >= start.z + size.z)
		return false;

	return true;
}

bool OctreeNode::checkIfNodeIsTotallyInside(const Vec3& other_start, const Vec3& other_size){
	bool check_x = start.x >= other_start.x && start.x + size.x <= other_start.x + other_size.x;
	bool check_y = start.y >= other_start.y && start.y + size.y <= other_start.y + other_size.y;
	bool check_z = start.z >= other_start.z && start.z + size.z <= other_start.z + other_size.z;

	return check_x && check_y && check_z;
}

Octree::Octree(size_t max_elements, size_t max_depth, const Vec3& max_start, const Vec3& max_size){
	this->max_elements = max_elements;
	this->max_depth = max_depth;
	this->max_start = max_start;
	this->max_size = max_size;
}

void Octree::build(const std::vector<BrushBuilder *>& brushes){
	for(BrushBuilder *brush : brushes){
		insert(brush);
	}

	printf("%lu\n", nodes.size());
}

void Octree::insert(BrushBuilder *brush){
	Vec3 brush_start, brush_end;

	std::stack<size_t> next_nodes;
	
	next_nodes.push(0);

	brush->getBoundingBox(&brush_start, &brush_end);

	if(nodes.size() == 0){
		OctreeNode *node = new OctreeNode(max_start, max_size, 0, max_elements);
		nodes.push_back(node);
	}

	while(!next_nodes.empty()){
		size_t node_id = next_nodes.top();
		OctreeNode *current_node = nodes[node_id];
		next_nodes.pop();

		if(current_node->checkCollision(brush_start, brush_end)){
			if(current_node->child == -1){
				if(current_node->brushes.size() < max_elements || current_node->stop_subdivide || current_node->depth == max_depth){
					current_node->brushes.push_back(brush);
				}
				else{
					split(node_id);
					next_nodes.push(node_id);
				}
			}
			else{
				for(size_t i = 0; i < 8; i++){
					next_nodes.push(current_node->child + i);
				}
			}
		}
	}
}

bool Octree::findCollision(const Body3dComponent& body, std::unordered_set<BrushBuilder *> *found_intersections){
	if(nodes.size() == 0)
		return false;

	bool found_collision = false;

	Vec3 body_start = body.position - Vec3(1.0f, 0.0f, 1.0f) * body.cylinder.radius;
	Vec3 body_size = Vec3(2.0f * body.cylinder.radius, body.cylinder.height, 2.0f * body.cylinder.radius);

	std::stack<size_t> next_nodes;

	next_nodes.push(0);

	while(!next_nodes.empty()){
		OctreeNode *current_node = nodes[next_nodes.top()];
		next_nodes.pop();

		if(!current_node->checkCollision(body_start, body_size))
			continue;

		if(current_node->child != -1){
			for(size_t i = 0; i < 8; i++)
				next_nodes.push(current_node->child + i);

			continue;
		}

		for(BrushBuilder *brush : current_node->brushes){
			if(body.checkCollision(brush)){
				found_collision = true;

				if(found_intersections == NULL)
					return true;
				else
					found_intersections->insert(brush);
			}
		}
	}

	return found_collision;
}

std::vector<OctreeNode *>& Octree::getNodes(void){
	return nodes;
}

Octree::~Octree(void){
	for(OctreeNode *i : nodes){
		delete i;
	}
}

void Octree::split(size_t parent){
	OctreeNode *split_node = nodes[parent];

	split_node->stop_subdivide = true;

	for(BrushBuilder *brush : split_node->brushes){
		Vec3 brush_start, brush_size;
		brush->getBoundingBox(&brush_start, &brush_size);

		if(!split_node->checkIfNodeIsTotallyInside(brush_start, brush_size)){
			split_node->stop_subdivide = false;
			break;
		}
	}

	if(split_node->stop_subdivide){
		return;
	}
	
	split_node->child = nodes.size();

	for(size_t i = 0; i < 8; i++){
		Vec3 transform = Vec3(i & 1, (i & 2) >> 1, (i & 4) >> 2);
		Vec3 start = split_node->start;
		Vec3 size = split_node->size / 2.0f;

		OctreeNode *new_node = new OctreeNode(
				start + size * transform,
				size,
				split_node->depth + 1,
				max_elements
				);

		for(BrushBuilder *brush : split_node->brushes){
			Vec3 brush_start, brush_end;
			brush->getBoundingBox(&brush_start, &brush_end);

			if(new_node->checkCollision(brush_start, brush_end)){
				new_node->brushes.push_back(brush);
			}
		}

		nodes.push_back(new_node);
	}

	split_node->brushes.clear();
}

};
