#define LIBAPAC_IMPLEMENTATION
#include "../libapac/libapac.h"

int main(void)
{
    set_memory_func_ptrs(&malloc, &realloc, &free);

    apz_t a, b, c;

    apz_init_pos64(&a, 4, 10);
    apz_init_pos64(&b, 4, 10);
    apz_init_pos64(&c, 4, 0);

    apz_hl_add(&c, &a, &b);

    for (uint64_t i = 0; i < 4; i++)
    {
        printf("%llu\n", c.num_array[i]);
    }

    return 0;
}