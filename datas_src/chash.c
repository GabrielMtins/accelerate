#include "datas/chash.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* The internal carray will have the following structure:
 * the first byte tells if that space is being used or not;
 * the following smemb_key bytes stores the key data;
 * the following smemb_value bytes stores the value data */

static size_t Data_Hash(const uint8_t *data, size_t size);
static void CHash_ReadData(CHash *hash, size_t index, uint8_t *used, void *key, void *value);
static void CHash_WriteData(CHash *hash, size_t index, uint8_t used, void *key, void *value);

CHash * CHash_CreateBytes(size_t smemb_key, size_t smemb_value, size_t nmemb){
	CHash *hash;

	hash = (CHash *) malloc(sizeof(CHash));

	hash->table = CArray_CreateBytes(smemb_key + smemb_value + 1, nmemb);
	hash->smemb_key = smemb_key;
	hash->smemb_value = smemb_value;
	hash->custom_hash = NULL;

	/* zero out the memory */
	memset(hash->table->data, 0, hash->table->nmemb * hash->table->smemb);

	return hash;
}

void CHash_SetCustomHashFunction(CHash *hash, size_t (*custom_hash)(void *, size_t)){
	hash->custom_hash = custom_hash;
}

int CHash_Insert(CHash *hash, void *key, void *member){
	uint8_t is_used;
	size_t index;

	if(hash->custom_hash == NULL)
		index = Data_Hash(key, hash->smemb_key) % CArray_Size(hash->table);
	else
		index = hash->custom_hash(key, hash->smemb_key) % CArray_Size(hash->table);

	while(1){
		CHash_ReadData(hash, index, &is_used, NULL, NULL);

		if(!is_used)
			break;
		
		index++;

		/* It was not possible to insert into the hash */
		if(index >= CArray_Size(hash->table))
			return 0;
	}

	CHash_WriteData(hash, index, 1, key, member);

	return 1;
}

int CHash_Get(CHash *hash, void *key, void *member){
	uint8_t is_used;
	size_t index;
	uint8_t compare_key[hash->smemb_key];
	uint8_t returned_value[hash->smemb_value];

	if(hash->custom_hash == NULL)
		index = Data_Hash(key, hash->smemb_key) % CArray_Size(hash->table);
	else
		index = hash->custom_hash(key, hash->smemb_key) % CArray_Size(hash->table);

	while(1){
		CHash_ReadData(hash, index, &is_used, compare_key, returned_value);

		if(is_used == 0)
			return 0;

		/* the correct value was found, break out of the loop */
		if(memcmp(compare_key, key, hash->smemb_key) == 0)
			break;

		index++;

		/* it wasn't possible to find the key on the table */
		if(index >= CArray_Size(hash->table))
			return 0;
	}

	memcpy(member, returned_value, hash->smemb_value);

	return 1;
}

int CHash_ForEachValue(CHash *hash, void (*map)(void *)){
	uint8_t is_used;
	uint8_t member[hash->smemb_value];

	for(size_t i = 0; i < CArray_Size(hash->table); i++){
		CHash_ReadData(hash, i, &is_used, NULL, member);

		if(is_used)
			map(member);
	}

	return 1;
}

void CHash_Destroy(CHash *hash){
	CArray_Destroy(hash->table);
	free(hash);
}

static size_t Data_Hash(const uint8_t *data, size_t size){
	size_t hash = 0;

	for(size_t i = 0; i < size; i++){
		hash += data[i];
		hash += hash << 10;
		hash ^= hash >> 6;
	}

	hash += hash << 3;
	hash ^= hash >> 11;
	hash += hash << 15;

	return hash;
}

static void CHash_ReadData(CHash *hash, size_t index, uint8_t *used, void *key, void *value){
	uint8_t data[1 + hash->smemb_key + hash->smemb_value];

	CArray_GetMember(hash->table, index, data);

	if(used != NULL)
		*used = data[0];

	if(key != NULL)
		memcpy(key, data + 1, hash->smemb_key);

	if(value != NULL)
		memcpy(value, data + 1 + hash->smemb_key, hash->smemb_value);
}

static void CHash_WriteData(CHash *hash, size_t index, uint8_t used, void *key, void *value){
	uint8_t data[1 + hash->smemb_key + hash->smemb_value];

	data[0] = used;

	memcpy(data + 1, key, hash->smemb_key);

	memcpy(data + 1 + hash->smemb_key, value, hash->smemb_value);

	CArray_SetMember(hash->table, index, data);
}
