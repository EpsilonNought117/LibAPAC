#ifndef APZ_DEF
#define APZ_DEF

/*
    GENERIC LIBC HEADERS
*/

#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <limits.h>
#include <string.h>

/*
    AMD64 AND x86_64 INTRINSICS HEADERS
*/

#include <immintrin.h>
#include <ammintrin.h>

/*
    ARM64 INTRINSICS HEADERS
*/

#include <arm64intr.h>
#include <arm64_neon.h>

/*
    LIBAPAC HEADERS
*/

#include "../Libapac_memory_allocator/LibAPAC_memory_alloc.h"
#include "../LIbAPAC_erros.h"

typedef uint64_t apz64;

#define APZ_ZPOS            0
#define APZ_NEG             1
#define APZ_NOT_IN_USE      2

#define POW_10_TO_19   1000 * 1000 * 1000 * 1000 * 1000 * 1000 * 10  // 10^19

typedef enum apz_base
{
    // hard to convert base

    DECIMAL = 10,

    // easy to convert base

    OCTAL = 8,          
    BASE32 = 32,
    BASE64 = 64,

    // easiest base to convert

    HEXADECIMAL = 16

}   apz_base;

typedef struct apz_t
{
    apz64* num_array;

    /*
        stores the arbitrary precision integer in an array of 64 bit unsigned integers
    */

    apz64 seg_alloc, seg_in_use;

    /*
        seg_alloc -> number of segments allocated currently for the arbitrary precision integer

        seg_in_use -> number of segments in use out of the total allocated
    */

    apz64 is_negative; // denotes whether the number is zero/positive or negative

} apz_t;

#endif