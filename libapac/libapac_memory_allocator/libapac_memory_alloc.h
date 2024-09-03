#ifndef LIBAPAC_MEMORY_ALLOCATOR
#define LIBAPAC_MEMORY_ALLOCATOR

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

void* (*malloc_ptr)(size_t init_size);
void* (*realloc_ptr)(void* old_buffer, size_t new_size);
void (*free_ptr)(void* memory, size_t free_size);

void stdlib_free_wrapper(void* memory, size_t free_size);

void set_memory_func_ptrs(
	void* (*func_ptr1)(size_t init_size),
	void* (*func_ptr2)(void* old_buffer, size_t new_size),
	void (*func_ptr3)(void* memory, size_t free_size)
);

#endif