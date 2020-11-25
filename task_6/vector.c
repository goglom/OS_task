#include "vector.h"
#include <stdlib.h>

#define __VEC_INIT_CAP 2
#define __VEC_INCR_FAC (float)1.5

bool vector_init(vector_t* vector)
{
	vector->size = 0;
	vector->capacity = __VEC_INIT_CAP;
	vector->array = calloc(vector->capacity, sizeof(stored_t));

	return vector->array == NULL;
}

bool vector_resize(vector_t* vector)
{
	size_t new_capacity = vector->size * __VEC_INCR_FAC;
	stored_t* temp =
			realloc(vector->array, new_capacity * sizeof(stored_t));

	if (temp == NULL)
		return true;

	vector->capacity = new_capacity;
	vector->array = temp;

	return false;
}

bool vector_push_back(vector_t* vector, stored_t value)
{
	if (vector->size == vector->capacity)
	{
		if (vector_resize(vector))
			return true;
	}
	vector->array[vector->size++] = value;

	return false;
}

stored_t vector_get(vector_t* vector, size_t index)
{
	return vector->array[index];
}

void vector_destroy(vector_t* vector)
{
	free(vector->array);
	*vector = (vector_t){ .size = 0, .capacity = 0, .array = NULL };
}
