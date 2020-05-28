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
    int n = 1;

    if (nb == 0 || nb == 1) {
        return 1;
    } else if (nb < 0) {
        return 0;
    }
    for (int i = 1; i <= nb; ++i) {
        n *= i;
    }
    return n;
}

Test(__TEST_NAME__, easy)
{
    TRY(0);
    TRY(1);
    TRY(-1);
}

Test(__TEST_NAME__, yes)
{
    TRY(10);
    TRY(2);
    TRY(12);
}

Test(__TEST_NAME__, charge)
{
    int n;

    srand(time(NULL));
    for (size_t i = 0; i < CHARGE_LOAD; ++i) {
        n = -12 + rand() % 24;
        TRY(n);
    }
}