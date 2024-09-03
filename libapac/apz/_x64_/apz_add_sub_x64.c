#include "apz_add_sub_x64.h"

void apz_abs_add_x64(apz_t *result, const apz_t *op1, const apz_t *op2)
{
    assert(result && op1 && op2);
    assert(result->num_array && op1->num_array && op2->num_array);

    apz_t *min; // points to the big int with lesser number of used limbs
    apz_t *max; // points to the big int with greater number of used limbs

    if (op1->seg_in_use > op2->seg_in_use)
    {
        max = op1;
        min = op2;
    }
    else
    {
        max = op2;
        min = op1;
    }

    uint64_t counter = 0; // counter to keep track of number of segments processed

    uint8_t carry = 0; // carry variable for adcx and adox respectively

    apz64 *temp_res = result->num_array, *temp_max = max->num_array, *temp_min = min->num_array;

    // process min->seg_in_use - 1 limbs

    while (counter < min->seg_in_use)
    {
        carry = _addcarry_u64(carry, temp_max[counter], temp_min[counter], temp_res + counter);
        counter++;
    }

    // process remaining limbs of max along with carry

    while (counter < max->seg_in_use)
    {
        carry = _addcarry_u64(carry, temp_max[counter], 0, temp_res + counter);
        counter++;
    }

    // check if carry is still left, if so then add it to final limb

    if (carry)
    {
        temp_res[counter] += carry;
    }

    result->seg_in_use = max->seg_in_use + (carry & 1);  
    
    /* (carry & 1) ensures limb count is increased if carry occured on last limb */ 

    return;
}

void apz_abs_sub_x64(apz_t *result, const apz_t *op1, const apz_t *op2)
{
    assert(result && op1 && op2);
    assert(result->num_array && op1->num_array && op2->num_array);

    // to produce correct results, assert following

    assert((op1->seg_in_use > op2->seg_in_use));                    
    assert((op1->seg_in_use == op2->seg_in_use) && (op1->num_array[op1->seg_in_use - 1] > op2->num_array[op2->seg_in_use - 1]));

    apz_t *max = op1;
    apz_t *min = op2;

    uint64_t counter = 0; // counter to keep track of number of segments processed

    uint8_t borrow = 0; // borrow variable to keep track of

    apz64 *temp_res = result->num_array, *temp_max = max->num_array, *temp_min = min->num_array;

    // process min->seg_in_use - 1 limbs

    while (counter < min->seg_in_use)
    {
        borrow = _subborrow_u64(borrow, temp_max[counter], temp_min[counter], temp_res + counter);
        counter++;
    }

    // process last borrow

    if (max->seg_in_use > min->seg_in_use)
    {
        temp_res[counter] = temp_max[counter]; 
        temp_res[counter] -= borrow;
        counter++;
    }

    // copy the remaining limbs

    while (counter < max->seg_in_use)
    {
        temp_res[counter] = temp_max[counter];
        counter++;
    }

    // change size to max->seg_in_use

    result->seg_in_use = max->seg_in_use;
    return; 
}