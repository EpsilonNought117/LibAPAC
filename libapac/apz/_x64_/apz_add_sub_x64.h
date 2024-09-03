#ifndef APZ_ADD_SUB_X64
#define APZ_ADD_SUB_X64

#include "../_apz_def.h"

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
 * @note caller has to make sure abs(op1) > abs(op2), otherwise asserted by function
 */
void apz_abs_sub_x64(apz_t *result, const apz_t *op1, const apz_t *op2);

#endif