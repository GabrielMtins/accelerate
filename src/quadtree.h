#ifndef QUADTREE_H
#define QUADTREE_H

/* Quadtree structure to calculate collisions in nlog(n) time complexity*/

#define ENTITY_PER_LEAF 32
#define MAX_DEPTH_TREE 10

#include "datas/carray.h"
#include "rect.h"
#include "entity.h"

typedef struct quadleaf_s{
	CArray *entities;
	size_t number_entities;
	Rect boundary;
	size_t depth;

	struct quadleaf_s *child[4];
} QuadNode;

typedef struct{
	QuadNode *root;
} QuadTree;

QuadNode * QuadNode_Create(Rect boundary, size_t depth);

void QuadNode_Split(QuadNode *node);

void QuadNode_Insert(QuadNode *node, Entity *member);

Entity * QuadNode_FindCollision(QuadNode *node, Entity *member);

void QuadNode_Destroy(QuadNode **leaf);

QuadTree * QuadTree_Create(Rect boundary);

void QuadTree_Insert(QuadTree *tree, Entity *member);

Entity * QuadTree_FindCollision(QuadTree *tree, Entity *member);

void QuadTree_Reset(QuadTree *tree);

void QuadTree_Destroy(QuadTree *tree);

#endif
