#ifndef APZ_ADD_SUB_X64
#define APZ_ADD_SUB_X64

#include "../_apz_def.h"

/// @note low level addition function for x64 architecture
/// @note result = abs(op1) + abs(op2)
/// @note result sign value set to APZ_ZPOS by default
void apz_add_x64(apz_t* result, apz_t * op1, apz_t* op2);

/// @note low level subtraction function for x64 architecture
/// @note result = abs(op1) - abs(op2)
/// @note result sign value set to APZ_ZPOS by default
void apz_sub_x64(apz_t* result, apz_t* op1, apz_t* op2);

#endif