#include "apz_op_limit.h"

apz64 apz_limit_add(apz_t* op1, apz_t* op2)
{
	return (op1->seg_in_use > op2->seg_in_use ? op1->seg_in_use + 1 : op2->seg_in_use + 1);
}

apz64 apz_limit_mul(apz_t* op1, apz_t* op2)
{
	return (op1->seg_in_use + op2->seg_in_use + 1);
}

apz64 apz_limit_sqr(apz_t* op1)
{
	return (2 * op1->seg_in_use);
}

apz64 apz_limit_exp(apz_t* op1, apz_t* op2)
{
	return (op1->seg_in_use * op2->seg_in_use);
}