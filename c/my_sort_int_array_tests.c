__CODE__

#include <criterion/criterion.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "lvector.h"

#define TRY(array, n)                        \
    __FUNCTION_NAME__(array, n);                        \
    for (size_t __i__ = 0; __i__ < n - 1; ++__i__)      \
        cr_assert(array[__i__] <= array[__i__ + 1]);    \

#define CHARGE_LOAD __ITERATIONS__

Test(__TEST_NAME__, empty)
{
    __FUNCTION_NAME__((int *)0xba0bab, 0);
}

Test(__TEST_NAME__, one_value)
{
    int a = 5;
    __FUNCTION_NAME__(&a, 1);
}

Test(__TEST_NAME__, two_values)
{
    lvector(int) v = {0};

    lvector_push_back(v, 1);
    lvector_push_back(v, -1);
    TRY(v.arr, v.len);
}

Test(__TEST_NAME__, charge)
{
    lvector(int) v;
    int value;

    lvector_create(v, CHARGE_LOAD, NULL);
    srand(time(NULL));
    for (size_t i = 0; i < CHARGE_LOAD; ++i) {
        value = 100000 - rand() % 200000;
        lvector_push_back(v, value);
    }
    TRY(v.arr, v.len);
}