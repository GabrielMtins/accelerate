#include "datas/cheap.h"

#include <stdio.h>
#include <stdlib.h>

CHeap * CHeap_CreateBytes(size_t smemb, size_t nmemb, int (*compare)(const void *, const void *)){
	CHeap *heap;

	heap = (CHeap *) malloc(sizeof(CHeap));

	heap->heap_data = CArray_CreateBytes(smemb, nmemb);
	heap->compare = compare;
	heap->top = 0;

	return heap;
}

int CHeap_IsEmpty(CHeap *heap){
	if(heap == NULL) return 0;

	return heap->top == 0;
}

int CHeap_GetTop(CHeap *heap, void *member){
	if(heap == NULL) return 0;

	return CArray_GetMember(heap->heap_data, 0, member);
}

int CHeap_Insert(CHeap *heap, void *new_member){
	if(heap == NULL) return 0;
	if(heap->top >= heap->heap_data->nmemb) return 0;

	size_t parent_index, child_index;
	void *parent_ref, *child_ref;

	parent_ref = child_ref = NULL;

	CArray_SetMember(heap->heap_data, heap->top, new_member);

	child_index = heap->top;

	heap->top++;
	
	while(1){
		if(child_index == 0) break;

		parent_index = (child_index - 1) / 2;

		parent_ref = CArray_GetReference(heap->heap_data, parent_index);
		child_ref = CArray_GetReference(heap->heap_data, child_index);

		if(heap->compare(parent_ref, child_ref) < 0){
			CArray_Swap(heap->heap_data, parent_index, child_index);
			child_index = parent_index;
		}
		else break;
	}

	return 1;
}

int CHeap_Remove(CHeap *heap, void *get_member){
	size_t parent_index;
	size_t child_index1, child_index2;
	int diff1, diff2;
	void *parent_ref, *child_ref1, *child_ref2;

	if(heap == NULL) return 0;
	if(heap->top == 0) return 0;

	CArray_GetMember(heap->heap_data, 0, get_member);
	heap->top--;
	CArray_Swap(heap->heap_data, 0, heap->top);

	parent_index = 0;

	while(1){
		child_index1 = 2 * parent_index + 1;
		child_index2 = 2 * parent_index + 2;

		parent_ref = CArray_GetReference(heap->heap_data, parent_index);
		child_ref1 = CArray_GetReference(heap->heap_data, child_index1);
		child_ref2 = CArray_GetReference(heap->heap_data, child_index2);

		if(child_index1 >= heap->top) break; /* if the two children doesn't exist */
		else if(child_index2 >= heap->top){ /* if only the first one exists */
			if(heap->compare(parent_ref, child_ref1) < 0){
				CArray_Swap(heap->heap_data, parent_index, child_index1);
				parent_index = child_index1;
			}
			else break;
		}
		else{ /* if both exist */
			diff1 = heap->compare(parent_ref, child_ref1);
			diff2 = heap->compare(parent_ref, child_ref2);

			if(diff1 < 0 && diff1 <= diff2){ /* if the parent is less than the first child */
				CArray_Swap(heap->heap_data, parent_index, child_index1);
				parent_index = child_index1;
			}
			else if(diff2 < 0 && diff2 <= diff1){ /* if the parent is less than the second child */
				CArray_Swap(heap->heap_data, parent_index, child_index2);
				parent_index = child_index2;
			}
			else{ /* if it is bigger than both */
				break;
			}
		}
	}

	return 1;
}

void CHeap_Destroy(CHeap *heap){
	CArray_Destroy(heap->heap_data);
	free(heap);
}
