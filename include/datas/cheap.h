#ifndef CHEAP_H
#define CHEAP_H

#include "carray.h"

#define CHeap_Create(type, nmemb, compare) CHeap_CreateBytes(sizeof(type), nmemb, compare)

/* By default it is a max heap */

typedef struct{
	CArray *heap_data;

	int (*compare)(const void *, const void *);
	/* This function returns a number less than 0, if the first argument is
	 * less than the second argument. Returns 0 if both arguments are equal.
	 * And it returns a number bigger than 0 if the first argument is more
	 * than the second argument */

	/* Since it is by default a max heap, you can invert the sign of the
	 * compare function */

	size_t top;
} CHeap;

/* The heap has members with smemb size in bytes, nmemb members at max and it uses a compare function
 * to sort it out internally */
CHeap * CHeap_CreateBytes(size_t smemb, size_t nmemb, int (*compare)(const void *, const void *));

/* Returns 1 if it's empty, 0 otherwise. */
int CHeap_IsEmpty(CHeap *heap);

/* All functions below return 1 at sucess, 0 otherwise */
/* Copies the member of the top */
int CHeap_GetTop(CHeap *heap, void *member);

/* Inserts a new member into the heap */
int CHeap_Insert(CHeap *heap, void *new_member);

/* Removes the biggest element from the list */
int CHeap_Remove(CHeap *heap, void *get_member);

/* Frees the memory used by the heap */
void CHeap_Destroy(CHeap *heap);

#endif
