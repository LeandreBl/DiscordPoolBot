__CODE__

#include <criterion/criterion.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#define TRY(...) cr_assert(__FUNCTION_NAME__(__VA_ARGS__) == __FUNCTION_NAME__dummy(__VA_ARGS__))

#define CHARGE_LOAD __ITERATIONS__
#define MAX_LEN 3000

int __FUNCTION_NAME__dummy(int nb, int p)
{
    return (int)pow(nb, p);
}

Test(__TEST_NAME__, easy)
{
    TRY(1, 1);
    TRY(2, 2);
    TRY(7, -1);
    TRY(-47, 2);
}

Test(__TEST_NAME__, tricky)
{
    TRY(0, 0);
    TRY(-1, -1);
    TRY(1, 0);
}

Test(__TEST_NAME__, charge)
{
    int nb;
    int n;

    srand(time(NULL));
    for (size_t i = 0; i < CHARGE_LOAD; ++i) {
        nb = rand() % 10;
        n = rand() % 10;
        TRY(nb, n);
    }
}