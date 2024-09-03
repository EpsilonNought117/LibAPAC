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

#include <intrin.h>
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

#include "../libapac_memory_allocator/libapac_memory_alloc.h"
#include "../libapac_erros.h"

typedef uint64_t apz64;

#define APZ_ZPOS            0
#define APZ_NEG             1

#define POW_10_TO_19   1000 * 1000 * 1000 * 1000 * 1000 * 1000 * 10  // 10^19

typedef enum apz_base
{
    
    DECIMAL = 10,   // computationally expensive to convert base

    OCTAL = 8,      // easy to convert base     
    BASE32 = 32,    // easy to convert base
    BASE64 = 64,    // easy to convert base

    HEXADECIMAL = 16 // easiest base to convert

}   apz_base;

typedef struct apz_t
{
    /**
    *   stores the arbitrary precision integer in an array of 64 bit unsigned integers
    */
    apz64* num_array;

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

#endif