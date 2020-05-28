__CODE__

#include <criterion/criterion.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#define TRY(...) cr_assert(__FUNCTION_NAME__(__VA_ARGS__) == __FUNCTION_NAME__dummy(__VA_ARGS__))

#define CHARGE_LOAD __ITERATIONS__
#define MAX_LEN 3000

int __FUNCTION_NAME__dummy(int nb)
{
    int i;

    if (nb < 0)
        nb = -nb;
    for (i = 2; i <= nb - 1; i++)
    {
        if (nb % i == 0)
            return 0;
    }
    return i == nb;
}

Test(__TEST_NAME__, easy)
{
    TRY(0);
    TRY(3);
    TRY(5);
    TRY(7);
}

Test(__TEST_NAME__, tricky)
{
    TRY(-1);
    TRY(-2);
    TRY(2);
    TRY(1);
}

Test(__TEST_NAME__, charge)
{
    int nb;

    srand(time(NULL));
    for (size_t i = 0; i < CHARGE_LOAD; ++i) {
        nb = -500 + rand() % 1000;
        TRY(nb);
    }
}