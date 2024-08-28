#include "apz_init.h"

libapac_err apz_init(apz_t* result, size_t init_size)
{
	assert(result && init_size);

	result->num_array = malloc_ptr(sizeof(apz64) * init_size);

	if (!result->num_array)
	{
		fprintf(stderr, "Memory allocation failed in 'apz_init'!");
		return LIBAPAC_OOM;
	}

	result->seg_alloc = init_size;
	result->num_array = memset(result->num_array, result->seg_alloc, sizeof(apz64));

	result->seg_in_use = 0;
	result->is_negative = APZ_NOT_IN_USE;

	return LIBAPAC_OKAY;
}

libapac_err apz_init_u64(apz_t* result, size_t init_size, uint64_t init_value)
{
	assert(result && init_size);

	result->num_array = (apz64)malloc_ptr(init_size * sizeof(apz64));

	if (!result->num_array)
	{
		fprintf(stderr, "Memory allocation failed in 'apz_init_u64'!");
		return LIBAPAC_OOM;
	}

	result->seg_alloc = init_size;
	result->num_array = memset(result->num_array, result->seg_alloc, sizeof(apz64));

	result->num_array[0] = init_value;
	result->seg_in_use = 1;
	result->is_negative = APZ_ZPOS;
	
	return LIBAPAC_OKAY;
}

libapac_err apz_init_i64(apz_t* result, size_t init_size, int64_t init_value)
{
	assert(result && init_size && init_value);

	result->num_array = (apz64)malloc_ptr(init_size * sizeof(apz64));

	if (!result->num_array)
	{
		fprintf(stderr, "Memory allocation failed in 'apz_init_i64'!");
		return LIBAPAC_OOM;
	}

	result->seg_alloc = init_size;
	result->num_array = memset(result->num_array, result->seg_alloc, sizeof(apz64));

	apz64 abs_value = (init_value ^ (init_value >> 63)) - (init_value >> 63);   // calculate absolute value of init_value

	result->num_array[0] = abs_value;
	result->seg_in_use = 1;
	result->is_negative = APZ_NEG;

	return LIBAPAC_OKAY;
}

libapac_err apz_grow(apz_t* result, size_t new_size)
{
	assert(result && (new_size > result->seg_alloc));
	
	apz64* temp_mem = realloc_ptr(result->num_array, new_size * sizeof(apz64));

	if (!temp_mem)
	{
		fprintf(stderr, "Memory allocation failed in 'apz_grow'!");
		return LIBAPAC_OOM;
	}

	result->num_array = temp_mem;
	result->seg_alloc = new_size;
	temp_mem = NULL;

	return LIBAPAC_OKAY;
}

libapac_err apz_shrink_fit(apz_t* result)
{
	assert(result && result->num_array);

	apz64* temp_mem = realloc_ptr(result->num_array, sizeof(apz64) * result->seg_in_use);

	if (!temp_mem)
	{
		fprintf(stderr, "Memory allocation failed in 'apz_grow'!");
		return LIBAPAC_OOM;
	}

	result->num_array = temp_mem;
	result->seg_alloc = result->seg_in_use;
	temp_mem = NULL;

	return LIBAPAC_OKAY;
}

libapac_err  apz_free(apz_t* result)
{
	assert(result && result->num_array);

	free_ptr(result->num_array, result->seg_alloc);

	result->num_array = NULL;
	result->seg_alloc = 0;
	result->seg_in_use = 0;
	result->is_negative = APZ_NOT_IN_USE;

	return LIBAPAC_OKAY;
}