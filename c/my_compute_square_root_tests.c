__CODE__

#include <criterion/criterion.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define TRY(...) cr_assert(__FUNCTION_NAME__(__VA_ARGS__) == __FUNCTION_NAME__dummy(__VA_ARGS__))

#define CHARGE_LOAD __ITERATIONS__
#define MAX_LEN 3000

int __FUNCTION_NAME__dummy(int nb)
{
    int sq;

    if (nb < 0 || nb == 0)
        return 0;
    for (sq = 0; sq * sq < nb; ++sq);
    if (sq * sq == nb)
        return sq;
    return 0;
}

Test(__TEST_NAME__, easy)
{
    TRY(0);
    TRY(1);
    TRY(-1);
    TRY(-35441);
}

Test(__TEST_NAME__, yes)
{
    TRY(3 * 3);
    TRY(7 * 7);
    TRY(12 * 12);
}

Test(__TEST_NAME__, charge)
{
    int n;

    srand(time(NULL));
    for (size_t i = 0; i < CHARGE_LOAD; ++i)
    {
        n = rand();
        TRY(n);
    }
}