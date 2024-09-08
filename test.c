#define LIBAPAC_IMPLEMENTATION
#include "libapac.h"

int main(void)
{
    set_memory_func_ptrs(&malloc, &realloc, &free);

    apz_t a, b, c;

    apz_init_pos64(&a, 3, ULLONG_MAX);
    apz_init_neg64(&b, 3, ULLONG_MAX);
    apz_init_pos64(&c, 4, 0);

    apz_hl_add(&c, &a, &b);

    for (int i = 0; i < 4; i++)
    {
        printf("%llu\n", c.num_array[i]);
    }

    printf("%llu\n", c.is_negative);

    return 0;
}