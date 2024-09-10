#include "quadtree.h"

#include <stdio.h>
#include <stdlib.h>

QuadNode * QuadNode_Create(Rect boundary, size_t depth){
	QuadNode *node;

	node = (QuadNode *) malloc(sizeof(QuadNode));

	node->entities = CArray_Create(Entity *, ENTITY_PER_LEAF);
	node->number_entities = 0;
	node->boundary = boundary;
	node->depth = depth;

	for(int i = 0; i < 4; i++)
		node->child[i] = NULL;

	return node;
}

void QuadNode_Split(QuadNode *node){
	Entity *tmp;

	node->child[0] = QuadNode_Create(
			Rect_Create(
				node->boundary.x,
				node->boundary.y,
				node->boundary.w / 2,
				node->boundary.h / 2
				),
			node->depth + 1
			);

	node->child[1] = QuadNode_Create(
			Rect_Create(
				node->boundary.x + node->boundary.w / 2,
				node->boundary.y,
				node->boundary.w / 2,
				node->boundary.h / 2
				),
			node->depth + 1
			);

	node->child[2] = QuadNode_Create(
			Rect_Create(
				node->boundary.x,
				node->boundary.y + node->boundary.h / 2,
				node->boundary.w / 2,
				node->boundary.h / 2
				),
			node->depth + 1
			);

	node->child[3] = QuadNode_Create(
			Rect_Create(
				node->boundary.x + node->boundary.w / 2,
				node->boundary.y + node->boundary.h / 2,
				node->boundary.w / 2,
				node->boundary.h / 2
				),
			node->depth + 1
			);

	for(int i = 0; i < ENTITY_PER_LEAF; i++){
		CArray_GetMember(node->entities, i, &tmp);
		QuadNode_Insert(node, tmp);
	}

	CArray_Destroy(node->entities);
}

void QuadNode_Insert(QuadNode *node, Entity *member){
	if(node->child[0] == NULL){ /* if it is a leaf node */
		if(node->number_entities >= ENTITY_PER_LEAF && node->depth < MAX_DEPTH_TREE){ /* handle overflow */
			QuadNode_Split(node);
			QuadNode_Insert(node, member);
		}
		else{ /* Just insert the new member */
			if(node->number_entities >= CArray_Size(node->entities)) /* changes size */
				CArray_Resize(node->entities, CArray_Size(node->entities) + ENTITY_PER_LEAF);

			CArray_SetMember(node->entities, node->number_entities, &member);
			node->number_entities++;
		}
	}
	else{ /* if it is a parent node */
		for(int i = 0; i < 4; i++){
			if(Rect_Intersects(node->child[i]->boundary, Entity_GetRectHitbox(member))){
				QuadNode_Insert(node->child[i], member);
			}
		}
	}
}

Entity * QuadNode_FindCollision(QuadNode *node, Entity *member){
	Entity *tmp;

	if(node->child[0] == NULL){ /* if it is a leaf node */
		for(size_t i = 0; i < node->number_entities; i++){
			CArray_GetMember(node->entities, i, &tmp);
			
			if(tmp == member) continue;

			if(Entity_CheckCollision(member, tmp)){
				return tmp;
			}
		}
	}
	else{ /* if it is a parent node */
		for(int i = 0; i < 4; i++){
			if(Rect_Intersects(node->child[i]->boundary, Entity_GetRectHitbox(member))){
				tmp = QuadNode_FindCollision(node->child[i], member);

				if(tmp != NULL) return tmp;
			}
		}
	}

	return NULL;
}

void QuadNode_Destroy(QuadNode **node){
	QuadNode *deref;

	if(*node == NULL) return;

	deref = *node;

	for(int i = 0; i < 4; i++){
		QuadNode_Destroy(&deref->child[i]);
	}

	if(deref->entities != NULL)
		CArray_Destroy(deref->entities);

	free(deref);
	*node = NULL;
}

QuadTree * QuadTree_Create(Rect boundary){
	QuadTree *tree;

	tree = (QuadTree *) malloc(sizeof(QuadTree));

	tree->root = QuadNode_Create(boundary, 0);

	return tree;
}

void QuadTree_Insert(QuadTree *tree, Entity *member){
	QuadNode_Insert(tree->root, member);
}

Entity * QuadTree_FindCollision(QuadTree *tree, Entity *member){
	return QuadNode_FindCollision(tree->root, member);
}

void QuadTree_Reset(QuadTree *tree){
	Rect old_boundary;

	old_boundary = tree->root->boundary;

	QuadNode_Destroy(&tree->root);

	tree->root = QuadNode_Create(old_boundary, 0);
}

void QuadTree_Destroy(QuadTree *tree){
	QuadNode_Destroy(&tree->root);
	free(tree);
}
