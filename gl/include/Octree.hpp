#ifndef OCTREE_HPP
#define OCTREE_HPP

#include "BrushBuilder.hpp"
#include "Vec3.hpp"

#include <unordered_set>

namespace acc {

struct Body3dComponent;

struct OctreeNode {
	Vec3 start;
	Vec3 size;

	std::vector<BrushBuilder *> brushes;

	int child;
	size_t depth;

	OctreeNode(const Vec3& start, const Vec3& size, size_t depth, size_t max_elements);
	bool checkCollision(const Vec3& other_start, const Vec3& other_size);
};

class Octree {
	public:
		Octree(size_t max_elements, size_t max_depth, const Vec3& max_start, const Vec3& max_size);
		void build(const std::vector<BrushBuilder *>& brushes);
		void insert(BrushBuilder *brush);
		bool findCollision(const Body3dComponent& body, std::unordered_set<BrushBuilder *> *found_intersections);
		std::vector<OctreeNode *>& getNodes(void);
		~Octree(void);

	private:
		void split(size_t parent);

		std::vector<OctreeNode *> nodes;

		Vec3 max_start, max_size;
		size_t max_elements;
		size_t max_depth;
};

};

#endif
