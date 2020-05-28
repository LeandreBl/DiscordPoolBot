__CODE__

#include <criterion/criterion.h>
#include <stdlib.h>
#include <time.h>

#define CHARGE_LOAD __ITERATIONS__

Test(__TEST_NAME__, easy)
{
    int a = 5;
    int b = 1;

    __FUNCTION_NAME__(&a, &b);
    cr_assert(a == 1 && b == 5);
}

Test(__TEST_NAME__, negative)
{
    int a = 15;
    int b = -1;

    __FUNCTION_NAME__(&a, &b);
    cr_assert(a == -1 && b == 15);
}

Test(__TEST_NAME__, bigints)
{
    int a = 600;
    int b = -10000000;

    __FUNCTION_NAME__(&a, &b);
    cr_assert(a == -10000000 && b == 600);
}

Test(__TEST_NAME__, charge)
{
    int a;
    int b;
    int newa;
    int newb;

    srand(time(NULL));
    for (size_t i = 0; i < CHARGE_LOAD; ++i) {
        a = rand();
        b = rand();
        newa = b;
        newb = a;
        __FUNCTION_NAME__(&a, &b);
        cr_assert(a == newa && b == newb);
    }
}