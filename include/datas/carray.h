#ifndef CARRAY_H
#define CARRAY_H

#include <stdint.h>
#include <stddef.h>

/* A generic type array implementation */

/* Just a macro for conveniance sake */
#define CArray_Create(type, nmemb) CArray_CreateBytes(sizeof(type), nmemb)
#define CArray_Map(arr, new_type, func) CArray_MapBytes(arr, sizeof(new_type), func)

typedef struct{
	uint8_t *data;

	size_t nmemb; /* number of members */
	size_t smemb; /* byte size of each member */
} CArray;

/* Create an array with nmemb members and each member has smemb size in bytes */
CArray * CArray_CreateBytes(size_t smemb, size_t nmemb);

/* Returns the size of the array */
size_t CArray_Size(CArray *arr);

/* Returns a reference for the member at index position, NULL at case of failure */
void * CArray_GetReference(CArray *arr, size_t index);

/* Returns 1 in case of sucess and 0 otherwise.
 * Copies a member of the index to get_member pointer */
int CArray_GetMember(CArray *arr, size_t index, void *get_member);

/* Copies data from new_member pointer to the data inside the array */
int CArray_SetMember(CArray *arr, size_t index, void *new_member);

int CArray_Swap(CArray *arr, size_t index1, size_t index2);

int CArray_Resize(CArray *arr, size_t nmemb);

/* Returns 1 if is able to sort, 0 otherwise.
 * Sorts the array using standard qsort C function. 
 * The function compar returns a number less than 0 if the first argument
 * is less than the second. It equals zero when the first argument is equal
 * to the second and it returns a number greater than 0 if the first argument
 * is greater than the second */
int CArray_Sort(CArray *arr, int (*compar)(const void *, const void *));

/* Use only if the array is sorted. Returns -1 if the element isn't found and
 * returns the index of the element on array if it is found. */
int CArray_BinarySearch(CArray *arr, void *member, int (*compar)(const void *, const void *));

/* Just some functions for using in functional programming */
CArray * CArray_MapBytes(CArray *arr, size_t new_smemb, void (*map)(void *, void *));

void CArray_Fold(CArray *arr, void (*fold)(void *, void *, void *), void *return_value);

void CArray_Destroy(CArray *arr);

#endif
