#ifndef _MY_VECTOR_
#define _MY_VECTOR_

#include <stddef.h>
#include <stdbool.h>
#include <sys/types.h>

typedef off_t stored_t;

typedef struct vector
{
	size_t size;
	size_t capacity;
	stored_t* array;
} vector_t;

bool vector_init(vector_t* vector);
bool vector_resize(vector_t* vector);
bool vector_push_back(vector_t* vector, stored_t value);
stored_t vector_get(vector_t* vector, size_t index);
void vector_destroy(vector_t* vector);
#endif
