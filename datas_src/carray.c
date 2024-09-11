#include "datas/carray.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

CArray * CArray_CreateBytes(size_t smemb, size_t nmemb){
	CArray *arr;

	arr = (CArray *) malloc(sizeof(CArray));

	arr->nmemb = nmemb;
	arr->smemb = smemb;

	if(nmemb != 0){
		arr->data = (uint8_t *) malloc(smemb * nmemb);
		memset(arr->data, 0, smemb * nmemb);
	}
	else
		arr->data = NULL;

	return arr;
}

size_t CArray_Size(CArray *arr){
	if(arr == NULL) return 0;

	return arr->nmemb;
}

void * CArray_GetReference(CArray *arr, size_t index){
	if(arr == NULL) return NULL;
	if(arr->data == NULL) return NULL;
	if(index >= arr->nmemb) return NULL;

	return (void *) (arr->data + index * arr->smemb);
}

int CArray_GetMember(CArray *arr, size_t index, void *get_member){
	if(arr == NULL) return 0;
	if(arr->data == NULL) return 0;
	if(index >= arr->nmemb) return 0;

	/* copy data */
	memcpy(get_member, arr->data + index * arr->smemb, arr->smemb);

	return 1;
}

int CArray_SetMember(CArray *arr, size_t index, void *new_member){
	if(arr == NULL) return 0;
	if(arr->data == NULL) return 0;

	if(index >= arr->nmemb) return 0;

	/* copy data */
	memcpy(arr->data + index * arr->smemb, new_member, arr->smemb);

	return 1;
}

int CArray_Swap(CArray *arr, size_t index1, size_t index2){
	uint8_t member1[arr->smemb], member2[arr->smemb];

	CArray_GetMember(arr, index1, member1);
	CArray_GetMember(arr, index2, member2);

	CArray_SetMember(arr, index2, member1);
	CArray_SetMember(arr, index1, member2);

	return 1;
}

int CArray_Resize(CArray *arr, size_t nmemb){
	if(arr == NULL) return 0;
	if(arr->data == NULL) return 0;

	arr->nmemb = nmemb;

	if(nmemb == 0){
		free(arr->data);
		arr->data = NULL;
	}
	else
		arr->data = realloc(arr->data, arr->nmemb * arr->smemb);

	return 1;
}

int CArray_Sort(CArray *arr, int (*compar)(const void *, const void *)){
	if(arr == NULL) return 0;
	if(arr->data == NULL) return 0;

	qsort(arr->data, arr->nmemb, arr->smemb, compar);

	return 1;
}

/* Use only if the array is sorted. Returns -1 if the element isn't found and
 * returns the index of the element on array if it is found. */
int CArray_BinarySearch(CArray *arr, void *member, int (*compar)(const void *, const void *)){
	size_t start, end, middle;
	int diff;

	start = 0;
	end = arr->nmemb - 1;

	while(start <= end){
		middle = (start + end) / 2;

		diff = compar(
				member,
				CArray_GetReference(arr, middle)
				);

		if(diff == 0)
			return middle;
		else if(diff > 0){
			start = middle + 1;
		}
		else{
			end = middle - 1;
		}
	}

	return -1;
}

CArray * CArray_MapBytes(CArray *arr, size_t new_smemb, void (*map)(void *, void *)){
	CArray *mapped_array = CArray_CreateBytes(new_smemb, arr->nmemb);

	uint8_t mapped_member[new_smemb];
	uint8_t old_member[arr->smemb];

	for(size_t i = 0; i < CArray_Size(arr); i++){
		CArray_GetMember(arr, i, old_member);

		map(old_member, mapped_member);
		
		CArray_SetMember(mapped_array, i, mapped_member);
	}

	return mapped_array;
}

void CArray_Fold(CArray *arr, void (*fold)(void *, void *, void *), void *return_value){
	uint8_t member[arr->smemb];
	uint8_t accumulator[arr->smemb];

	CArray_GetMember(arr, 0, accumulator);

	for(size_t i = 1; i < CArray_Size(arr); i++){
		CArray_GetMember(arr, i, member);

		fold(accumulator, member, accumulator);
	}

	memcpy(return_value, accumulator, arr->smemb);
}

void CArray_Destroy(CArray *arr){
	if(arr == NULL) return;
	if(arr->data != NULL) free(arr->data);

	free(arr);
}
