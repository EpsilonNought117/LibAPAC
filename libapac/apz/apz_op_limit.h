#ifndef APZ_OP_LIMIT
#define APZ_OP_LIMIT

#include "_apz_def.h"

apz64 apz_limit_add(apz_t* op1, apz_t* op2);

apz64 apz_limit_mul(apz_t* op1, apz_t* op2);

apz64 apz_limit_sqr(apz_t* op1);

apz64 apz_limit_exp(apz_t* op1, apz_t* op2);

apz64 apz_limit_mod_exp(apz_t* op1, apz_t* op2);

#endif