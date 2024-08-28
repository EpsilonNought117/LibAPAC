#include "LibAPAC_memory_alloc.h"

void stdlib_free_wrapper(void* memory, size_t free_size)
{
	return free(memory);
}

void set_memory_func_ptrs
(
	void* (*func_ptr1)(size_t init_size),
	void* (*func_ptr2)(void* old_buffer, size_t new_size),
	void (*func_ptr3)(void* memory, size_t free_size)
)
{
	if (!func_ptr1 && !func_ptr2 && !func_ptr3)
	{
		malloc_ptr = &malloc;
		realloc_ptr = &realloc;
		free_ptr = &stdlib_free_wrapper;
		return;
	}
	else if (func_ptr1 && func_ptr2 && func_ptr3)
	{
		malloc_ptr = func_ptr1;
		realloc_ptr = func_ptr2;
		free_ptr = func_ptr3;
		return;
	}
	else
	{
		assert("Function parameters not correct in 'set_memory_func_ptrs'!");
	}
}