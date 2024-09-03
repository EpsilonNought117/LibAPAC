#ifndef APZ_INIT
#define APZ_INIT

#include "_apz_def.h"

libapac_err apz_init_u64(apz_t* result, size_t init_size_limbs, uint64_t value);  // for unsigned 64 bit int

libapac_err apz_init_i64(apz_t* result, size_t init_size_limbs, int64_t value);   // for signed 64 bit int

libapac_err apz_grow(apz_t* result, size_t new_size_limbs);

libapac_err apz_shrink_fit(apz_t* result);

libapac_err apz_free(apz_t* result);   // makes result = NULL

#endif