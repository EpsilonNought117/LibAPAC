#ifndef LIBAPAC_H
#define LIBAPAC_H

/*
    LIBC HEADERS
*/

#include <stdlib.h>
#include <stdint.h>

#ifndef REPORT_ERR_FPRINTF
    #include <stdio.h>
    #define REPORT_ERR(stream, fmt, ...) fprintf(stream, fmt, ## __VA_ARGS__)
#endif

#ifndef ASSERT
    #include <assert.h>
    #define ASSERT(x) assert(x)

    /*
        user can get rid of this dependency if needed by redefining ASSERT(x)
    */

#endif

#include <limits.h>
#include <string.h>

/*
    AMD64 AND x64 INTRINSICS HEADERS
*/

#include <intrin.h>
#include <immintrin.h>
#include <ammintrin.h>

/*
    ERROR HANDLING
*/

typedef enum LIBAPAC_ERRORS
{
    LIBAPAC_OKAY,
    LIBAPAC_OOM,
    LIBAPAC_MATH_ERR,

} libapac_err;

/*
    LIBAPAC DYNAMIC MEMORY ALLOCATION FUNCTION
*/

void *(*malloc_ptr)(size_t init_size);
void *(*realloc_ptr)(void *old_buffer, size_t new_size);
void (*free_ptr)(void *memory, size_t free_size);

void stdlib_free_wrapper(void *memory, size_t free_size);

void set_memory_func_ptrs(void *(*func_ptr1)(size_t init_size), void *(*func_ptr2)(void *old_buffer, size_t new_size), void (*func_ptr3)(void *memory, size_t free_size));

/*
    ARBITRARY PRECISION INTEGER DEFINITION
*/

typedef uint64_t apz64;

#define APZ_ZPOS 0
#define APZ_NEG 1

#define POW_10_TO_19 1000 * 1000 * 1000 * 1000 * 1000 * 1000 * 10 // 10^19

typedef enum apz_base
{

    DECIMAL = 10, // computationally expensive to convert base

    OCTAL = 8,   // easy to convert base
    BASE32 = 32, // easy to convert base
    BASE64 = 64, // easy to convert base

    HEXADECIMAL = 16 // easiest base to convert

} apz_base;

typedef struct apz_t
{
    /**
     *   stores the arbitrary precision integer in an array of 64 bit unsigned integers
     */
    apz64 *num_array;

    /**
     *   seg_alloc -> number of segments allocated currently for the arbitrary precision integer
     */
    apz64 seg_alloc;

    /**
     *   seg_in_use -> number of segments in use out of the total allocated
     */
    apz64 seg_in_use;

    apz64 is_negative; // denotes whether the number is zero/positive or negative

} apz_t;

/**
 *  APZ INIT FUNCTIONS
 */

libapac_err apz_init_ui64(apz_t *result, size_t init_size_limbs, uint64_t value); // for unsigned 64 bit int

libapac_err apz_init_si64(apz_t *result, size_t init_size_limbs, int64_t value); // for signed 64 bit int

libapac_err apz_grow(apz_t *result, size_t new_size_limbs);

libapac_err apz_shrink_fit(apz_t *result);

libapac_err apz_free(apz_t *result); // makes result = NULL

/**
 * APZ ASSIGMENT FUNCTIONS
 */

libapac_err apz_copy(apz_t *result, apz_t *op1);

libapac_err apz_set_string(apz_t *result, uint8_t *string, uint64_t string_len);

/**
 *  APZ OPERATION LIMIT FUNCTIONS
 */

apz64 inline apz_limit_add(apz_t *op1, apz_t *op2);

apz64 inline apz_limit_mul(apz_t *op1, apz_t *op2);

apz64 inline apz_limit_sqr(apz_t *op1);

apz64 inline apz_limit_exp(apz_t *op1, apz_t *op2);

apz64 inline apz_limit_mod_exp(apz_t *op1, apz_t *op2);

/**
 *  APZ HIGH LEVEL BASIC ARITHMETIC FUNCTIONS
 */

/// @note result = op1 + op2
/// @note result sign set according to values of op1 and op2
libapac_err apz_hl_add(apz_t *result, const apz_t *op1, const apz_t *op2);

/// @note result = op1 + value
/// @note result sign set according to values of op1 and value
libapac_err apz_hl_add_ui(apz_t *result, const apz_t *op1, const uint64_t value); // result = op1 + value

/// @note result = op1 - op2
/// @note result sign set according to values of op1 and op2
libapac_err apz_hl_sub(apz_t *result, const apz_t *op1, const apz_t *op2); // result = op1 - op2

/// @note result = op1 - value
/// @note result sign set according to values of op1 and value
libapac_err apz_hl_sub_ui(apz_t *result, const apz_t *op1, const uint64_t value); // result = op1 - value

/// @note result = value - op1
/// @note result sign set according to values of op1 and value
libapac_err apz_hl_ui_sub(apz_t *result, const uint64_t value, const apz_t *op1); // result = value - op1

libapac_err apz_hl_mul(apz_t *result, const apz_t *op1, const apz_t *op2); // result = op1 * op2

libapac_err apz_hl_mul_ui(apz_t *result, const apz_t *op1, const uint64_t value); // result = op1 * value

libapac_err apz_hl_mul_si(apz_t *result, const apz_t *op1, const int64_t value); // result = op1 * value (value is signed)

/**
 *  APZ LOW LEVEL FUNCTIONS SPECIFIC TO x64 ARCH
 */

/**
 * Does not perform any memory allocation on result
 * @result result = abs(op1) + abs(op2)
 * 
 * @note result->is_negative set to APZ_ZPOS by default
 * @note caller has to make sure limb count is one more than maximum
 */
void apz_abs_add_x64(apz_t *result, const apz_t *op1, const apz_t *op2);

/**
 * @result result = abs(op1) - abs(op2)
 * 
 * @note result->is_negative set to APZ_ZPOS by default
 * @note caller has to make sure abs(op1) > abs(op2), otherwise behaviour is undefined
 */
void apz_abs_sub_x64(apz_t *result, const apz_t *op1, const apz_t *op2);

#endif

#define LIBAPAC_IMPLEMENTATION

#ifdef LIBAPAC_IMPLEMENTATION

/*
    LIBAPAC MEMORY ALLOCATION METHOD IMPLEMENTATION
*/

void stdlib_free_wrapper(void *memory, size_t free_size)
{
    return free(memory);
}

void set_memory_func_ptrs(
    void *(*func_ptr1)(size_t init_size),
    void *(*func_ptr2)(void *old_buffer, size_t new_size),
    void (*func_ptr3)(void *memory, size_t free_size))
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
        ASSERT("Function parameters not correct in 'set_memory_func_ptrs'!");
    }
}

/*
    APZ INIT FUNCTION DEFINITIONS
*/

libapac_err apz_init_ui64(apz_t *result, size_t init_size, uint64_t init_value)
{
    ASSERT(result && init_size && ((init_size >> 3) < ULLONG_MAX));

    apz64 alloc_size = (apz64)init_size << 3;

    result->num_array = (apz64 *)malloc_ptr(alloc_size);

    if (!result->num_array)
    {
        fprintf(stderr, "Memory allocation failed in 'apz_init_u64'!");
        return LIBAPAC_OOM;
    }

    result->seg_alloc = init_size;
    result->num_array = (apz64 *)memset(result->num_array, 0, result->seg_alloc * sizeof(apz64));

    result->num_array[0] = init_value;
    result->seg_in_use = 1;
    result->is_negative = APZ_ZPOS;

    return LIBAPAC_OKAY;
}

libapac_err apz_init_si64(apz_t *result, size_t init_size, int64_t init_value)
{
    ASSERT(result && init_size && ((init_size >> 3) < ULLONG_MAX));

    apz64 alloc_size = (apz64)init_size << 3;

    result->num_array = (apz64 *)malloc_ptr(alloc_size);

    if (!result->num_array)
    {
        fprintf(stderr, "Memory allocation failed in 'apz_init_i64'!");
        return LIBAPAC_OOM;
    }

    result->seg_alloc = init_size;
    result->num_array = (apz64 *)memset(result->num_array, 0, result->seg_alloc * sizeof(apz64));

    apz64 abs_value = (init_value ^ (init_value >> 63)) - (init_value >> 63); // calculate absolute value of init_value

    result->num_array[0] = abs_value;
    result->seg_in_use = 1;
    result->is_negative = APZ_NEG;

    return LIBAPAC_OKAY;
}

libapac_err apz_grow(apz_t *result, size_t new_size)
{
    ASSERT(result && (new_size > result->seg_alloc) && ((new_size >> 3) < ULLONG_MAX));

    apz64 *temp_mem = (apz64 *)realloc_ptr(result->num_array, new_size * sizeof(apz64));

    if (!temp_mem)
    {
        fprintf(stderr, "Memory allocation failed in 'apz_grow'!");
        return LIBAPAC_OOM;
    }

    apz64 *extended_addr = temp_mem + result->seg_alloc * sizeof(apz64);

    extended_addr = (apz64 *)memset(extended_addr, 0, new_size - result->seg_alloc);

    result->num_array = temp_mem;
    result->seg_alloc = new_size;
    temp_mem = NULL;

    return LIBAPAC_OKAY;
}

libapac_err apz_shrink_fit(apz_t *result)
{
    ASSERT(result && result->num_array);

    apz64 *temp_mem = (apz64 *)realloc_ptr(result->num_array, sizeof(apz64) * result->seg_in_use);

    if (!temp_mem)
    {
        fprintf(stderr, "Memory allocation failed in 'apz_shrink_fit'!");
        return LIBAPAC_OOM;
    }

    result->num_array = temp_mem;
    result->seg_alloc = result->seg_in_use;
    temp_mem = NULL;

    return LIBAPAC_OKAY;
}

libapac_err apz_free(apz_t *result)
{
    ASSERT(result && result->num_array);

    free_ptr(result->num_array, result->seg_alloc * sizeof(apz64));

    result->num_array = NULL;

    free_ptr(result, sizeof(apz_t));

    return LIBAPAC_OKAY;
}

/*
    APZ OPERATION LIMIT FUNCTION DEFINITIONS
*/

apz64 inline apz_limit_add(apz_t *op1, apz_t *op2)
{
    return (op1->seg_in_use > op2->seg_in_use ? op1->seg_in_use + 1 : op2->seg_in_use + 1);
}

apz64 inline apz_limit_mul(apz_t *op1, apz_t *op2)
{
    return (op1->seg_in_use + op2->seg_in_use + 1);
}

apz64 inline apz_limit_sqr(apz_t *op1)
{
    return (2 * op1->seg_in_use);
}

apz64 inline apz_limit_exp(apz_t *op1, apz_t *op2)
{
    return (op1->seg_in_use * op2->seg_in_use);
}

/*
    APZ LOW LEVEL FUNCTIONS (x64 ARCH) IMPLEMENTATION
*/

void apz_abs_add_x64(apz_t *result, const apz_t *max_elem, const apz_t *min_elem)
{
    ASSERT(result && max_elem && min_elem);
    ASSERT(result->num_array && max_elem->num_array && min_elem->num_array);

    // assumption made by function

    ASSERT(max_elem->seg_in_use >= min_elem->seg_in_use);

    uint64_t counter = 0; // counter to keep track of number of segments processed

    uint8_t carry = 0; // carry variable for carry propagation

    apz64 *temp_res = result->num_array, *temp_max = max_elem->num_array, *temp_min = min_elem->num_array;

    // process min->seg_in_use - 1 limbs

    while (counter < min_elem->seg_in_use)
    {
        carry = _addcarry_u64(carry, temp_max[counter], temp_min[counter], temp_res + counter);
        counter++;

        /*
            TEST ASM WITH ADCX AND ADOX ALONG WITH LOOP UNROLLING
        */
    }

    // process remaining limbs of max along with carry

    while (counter < max_elem->seg_in_use)
    {
        carry = _addcarry_u64(carry, temp_max[counter], 0, temp_res + counter);
        counter++;

        /*
            TEST ASM WITH ADCX AND ADOX ALONG WITH LOOP UNROLLING, MAYBE LOOP UNROLLING WILL SUFFICE
        */
    }

    // check if carry is still left, if so then add it to final limb

    if (carry)
    {
        temp_res[counter] += carry;
    }

    result->seg_in_use = max_elem->seg_in_use + (carry & 1);
    result->is_negative = APZ_ZPOS;

    /* (carry & 1) ensures limb count is increased if carry occured on last limb */

    return;
}

void apz_abs_sub_x64(apz_t *result, const apz_t *max_elem, const apz_t *min_elem)
{
    ASSERT(result && max_elem && min_elem);
    ASSERT(result->num_array && max_elem->num_array && min_elem->num_array);

    /*
        NO CHECKS ARE PERFORMED HERE WHETHER abs(max_elem) > abs(min_elem)
        NEEDS TO BE CHECKED BY CALLER OR USE HIGHER LEVEL FUNCTIONS
    */

    uint64_t counter = 0; // counter to keep track of number of segments processed

    uint8_t borrow = 0; // borrow variable for borrow propagation

    apz64 *temp_res = result->num_array, *temp_max = max_elem->num_array, *temp_min = min_elem->num_array;

    // process min->seg_in_use - 1 limbs

    while (counter < min_elem->seg_in_use)
    {
        borrow = _subborrow_u64(borrow, temp_max[counter], temp_min[counter], temp_res + counter);  // possibly loop unroll
        counter++;
    }

    // process remaining limbs

    while (counter < max_elem->seg_in_use)
    {
        borrow = _subborrow_u64(borrow, temp_max[counter], 0, temp_res + counter);  // can try loop unrolling
        counter++;
    }

    // change size to max->seg_in_use

    result->seg_in_use = max_elem->seg_in_use;
    result->is_negative = APZ_ZPOS;
    return;
}

#endif