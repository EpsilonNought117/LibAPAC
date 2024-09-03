#ifndef APZ_ASSIGN
#define APZ_ASSIGN

#include "_apz_def.h"

libapac_err apz_copy(apz_t* result, apz_t* op1);

libapac_err apz_set_string(apz_t* result, uint8_t* string, uint64_t string_len);

#endif