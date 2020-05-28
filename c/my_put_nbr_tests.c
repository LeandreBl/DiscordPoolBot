__CODE__

#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include <time.h>
#include <stdlib.h>

#include "lvector.h"

#define CHARGE_LOAD __ITERATIONS__

Test(__TEST_NAME__, easy, .init = cr_redirect_stdout)
{
    __FUNCTION_NAME__(20);
    __FUNCTION_NAME__(5);
    __FUNCTION_NAME__(534254254);
    cr_assert_stdout_eq_str("%d%d%d", 20, 5, 534254254);
}

Test(__TEST_NAME__, negative, .init = cr_redirect_stdout)
{
    __FUNCTION_NAME__(-15);
    __FUNCTION_NAME__(-1);
    __FUNCTION_NAME__(-9924545);
    cr_assert_stdout_eq_str("%d%d%d", -15, -1, -9924545);
}

Test(__TEST_NAME__, zero, .init = cr_redirect_stdout)
{
    __FUNCTION_NAME__(-0);
    __FUNCTION_NAME__(0);
    __FUNCTION_NAME__(+0);
    cr_assert_stdout_eq_str("000");
}

Test(__TEST_NAME__, charge, .init = cr_redirect_stdout)
{
    lvector(int) v;
    char *str = "";
    int value;

    srand(time(NULL));
    lvector_create(v, CHARGE_LOAD, NULL);
    for (size_t i = 0; i < CHARGE_LOAD; ++i) {
        value = rand();
        lvector_push_back(v, value);
        __FUNCTION_NAME__(value);
    }
    lvector_foreach(pint, v) {
        if (cr_asprintf(&str, "%s%d", str, *pint) == -1) {
            cr_log_error("Internal error: string concatenation failed\n");
            return;
        }
    }
    cr_assert_stdout_eq_str(str);
    free(str);
    lvector_destroy(v);
}