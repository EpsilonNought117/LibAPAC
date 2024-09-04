#include "apz_add_sub_x64.h"

void apz_abs_add_x64(apz_t *result, const apz_t *max_elem, const apz_t *min_elem)
{
    assert(result && max_elem && min_elem);
    assert(result->num_array && max_elem->num_array && min_elem->num_array);

    // assumption made by function

    assert(max_elem->seg_in_use >= min_elem->seg_in_use);

    uint64_t counter = 0; // counter to keep track of number of segments processed

    uint8_t carry = 0; // carry variable for carry propagation

    apz64 *temp_res = result->num_array, *temp_max = max_elem->num_array, *temp_min = min_elem->num_array;

    // process min->seg_in_use - 1 limbs

    while (counter < min_elem->seg_in_use)
    {
        carry = _addcarry_u64(carry, temp_max[counter], temp_min[counter], temp_res + counter);
        counter++;

        /*
            TEST ASM WITH ADCX AND ADOX ALONG WITH LOOP UNROLLING
        */
    }

    // process remaining limbs of max along with carry

    while (counter < max_elem->seg_in_use)
    {
        carry = _addcarry_u64(carry, temp_max[counter], 0, temp_res + counter);
        counter++;

        /*
            TEST ASM WITH ADCX AND ADOX ALONG WITH LOOP UNROLLING, MAYBE LOOP UNROLLING WILL SUFFICE
        */
    }

    // check if carry is still left, if so then add it to final limb

    if (carry)
    {
        temp_res[counter] += carry;
    }

    result->seg_in_use = max_elem->seg_in_use + (carry & 1);
    result->is_negative = APZ_ZPOS;

    /* (carry & 1) ensures limb count is increased if carry occured on last limb */

    return;
}

void apz_abs_sub_x64(apz_t *result, const apz_t *max_elem, const apz_t *min_elem)
{
    assert(result && max_elem && min_elem);
    assert(result->num_array && max_elem->num_array && min_elem->num_array);

    /*
        NO CHECKS ARE PERFORMED HERE WHETHER abs(max_elem) > abs(min_elem)
        NEEDS TO BE CHECKED BY CALLER OR USE HIGHER LEVEL FUNCTIONS
    */

    uint64_t counter = 0; // counter to keep track of number of segments processed

    uint8_t borrow = 0; // borrow variable for borrow propagation

    apz64 *temp_res = result->num_array, *temp_max = max_elem->num_array, *temp_min = min_elem->num_array;

    // process min->seg_in_use - 1 limbs

    while (counter < min_elem->seg_in_use)
    {
        borrow = _subborrow_u64(borrow, temp_max[counter], temp_min[counter], temp_res + counter);  // possibly loop unroll
        counter++;
    }

    // process remaining limbs

    while (counter < max_elem->seg_in_use)
    {
        borrow = _subborrow_u64(borrow, temp_max[counter], 0, temp_res + counter);                 // possibly loop unroll
        counter++;
    }

    // change size to max->seg_in_use

    result->seg_in_use = max_elem->seg_in_use;
    result->is_negative = APZ_ZPOS;
    return;
}