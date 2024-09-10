#include "hashstr.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

HashStr HashStr_Create(const char *str){
	HashStr new_str;

	memset(new_str.str, 0, HASH_STR_SIZE + 1);

	strncpy(new_str.str, str, HASH_STR_SIZE);

	return new_str;
}

size_t HashStr_Hash(void *ptr, size_t n){
	char *str = ((HashStr *) ptr)->str;
	size_t hash = 0;
	(void) n;

	for(size_t i = 0; i < strlen(str); i++){
		hash += str[i];
		hash += hash << 10;
		hash ^= hash >> 6;
	}

	hash += hash << 3;
	hash ^= hash >> 11;
	hash += hash << 15;

	return hash;
}
