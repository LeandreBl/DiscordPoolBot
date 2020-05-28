__CODE__

#include <criterion/criterion.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#define TRY(str) cr_assert(__FUNCTION_NAME__(str) == atoi(str))

#define CHARGE_LOAD __ITERATIONS__

Test(__TEST_NAME__, positive)
{
    TRY("5");
    TRY("+5");
    TRY("000000015");
    TRY("510000000");
}

Test(__TEST_NAME__, negative)
{
    TRY("-5");
    TRY("-0");
    TRY("-0000001");
    TRY("-1000000");
    TRY("-212435435");
}

Test(__TEST_NAME__, long_string)
{
    TRY("215341143134531354354");
    TRY("-215341143134531354354");
}

Test(__TEST_NAME__, charge)
{
    int value;
    char buffer[256] = {0};

    srand(time(NULL));
    for (size_t i = 0; i < CHARGE_LOAD; ++i) {
        value = rand();
        snprintf(buffer, sizeof(buffer), "%d", value);
        TRY(buffer);
    }
}