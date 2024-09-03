#ifndef LIBAPAC
#define LIBAPAC

/*
*	ERROR CODES AND MEMORY ALLOCATOR HEADERS
*/

#include "libapac_erros.h"
#include "libapac_memory_allocator/libapac_memory_alloc.h"

/*
*	APZ HEADER FILES PLATFORM INDEPENDENT
*/

#include "apz/_apz_def.h"
#include "apz/apz_init.h"
#include "apz/apz_assign.h"
#include "apz/apz_op_limit.h"
#include "apz/apz_hl_basic_arithmetic.h"

/*
*	APZ HEADER FILES FOR x86_64
*/

#include "apz/_x64_/apz_add_sub_x64.h"
#include "apz/_x64_/apz_mul_x64.h"

#endif
