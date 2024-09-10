#ifndef HASHSTR_H
#define HASHSTR_H

#define HASH_STR_SIZE 256

#include <stddef.h>

typedef struct{
	char str[HASH_STR_SIZE + 1];
} HashStr;

HashStr HashStr_Create(const char *str);

size_t HashStr_Hash(void *ptr, size_t n);

#endif
