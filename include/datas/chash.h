#ifndef CHASH_H
#define CHASH_H

#include <stdint.h>
#include <stddef.h>

#include "datas/carray.h"

#define CHash_Create(key_type, value_type, nmemb) CHash_CreateBytes(sizeof(key_type), sizeof(value_type), nmemb)

typedef struct{
	CArray *table;

	/* custom hash function */
	size_t (*custom_hash)(void *, size_t);

	size_t smemb_key;
	size_t smemb_value;
} CHash;

CHash * CHash_CreateBytes(size_t smemb_key, size_t smemb_value, size_t nmemb);

void CHash_SetCustomHashFunction(CHash *hash, size_t (*custom_hash)(void *, size_t));

int CHash_Insert(CHash *hash, void *key, void *member);

int CHash_Get(CHash *hash, void *key, void *member);

int CHash_ForEachValue(CHash *hash, void (*map)(void *));

void CHash_Destroy(CHash *hash);

#endif
