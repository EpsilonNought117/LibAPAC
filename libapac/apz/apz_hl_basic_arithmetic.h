#ifndef APZ_HL_BASIC_ARITHMETIC
#define APZ_HL_BASIC_ARITHMETIC

#include "_apz_def.h"

/*
	HIGH LEVEL FUNCTIONS FOR BASIC ARITHMETIC
*/




// INTEGER ADDITION

/// @note result = op1 + op2
/// @note result sign set according to values of op1 and op2
libapac_err apz_hl_add(apz_t* result, const apz_t* op1, const apz_t* op2);  

/// @note result = op1 + value
/// @note result sign set according to values of op1 and value
libapac_err apz_hl_add_ui(apz_t* result, const apz_t* op1, const uint64_t value);  // result = op1 + value




// INTEGER SUBTRACTION

/// @note result = op1 - op2
/// @note result sign set according to values of op1 and op2
libapac_err apz_hl_sub(apz_t* result, const apz_t* op1, const apz_t* op2);  // result = op1 - op2

/// @note result = op1 - value
/// @note result sign set according to values of op1 and value
libapac_err apz_hl_sub_ui(apz_t* result, const apz_t* op1, const uint64_t value);  // result = op1 - value

/// @note result = value - op1
/// @note result sign set according to values of op1 and value
libapac_err apz_hl_ui_sub(apz_t* result, const uint64_t value, const apz_t* op1);  // result = value - op1




// INTEGER MULTIPLICATION (SIGNED AND UNSIGNED)

libapac_err apz_hl_mul(apz_t* result, const apz_t* op1, const apz_t* op2);  // result = op1 * op2

libapac_err apz_hl_mul_ui(apz_t* result, const apz_t* op1, const uint64_t value);  // result = op1 * value

libapac_err apz_hl_mul_si(apz_t* result, const apz_t* op1, const int64_t value);   // result = op1 * value (value is signed)

#endif