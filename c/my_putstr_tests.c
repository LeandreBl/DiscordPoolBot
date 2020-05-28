__CODE__

#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include <time.h>
#include <stdlib.h>

#include "lvector.h"

#define CHARGE_LOAD __ITERATIONS__
#define MAX_LEN 3000

#define TRY(str) \
    __FUNCTION_NAME__(str);     \
    cr_assert_stdout_eq_str(str);

Test(__TEST_NAME__, easy, .init = cr_redirect_stdout)
{
    TRY("Je suis une string sexy");
}

Test(__TEST_NAME__, empty, .init = cr_redirect_stdout)
{
    //TRY("");
}

Test(__TEST_NAME__, escape, .init = cr_redirect_stdout)
{
    TRY("\n\a\r\bhelloworld****");
}

void str_free(char **pstr)
{
    free(*pstr);
}

Test(__TEST_NAME__, charge, .init = cr_redirect_stdout)
{
    lvector(char *) v;
    const char ALPHANUM[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    size_t len;
    char *str;

    srand(time(NULL));
    lvector_create(v, CHARGE_LOAD, str_free);
    for (size_t step = 0; step < CHARGE_LOAD; ++step) {
        len = rand() % MAX_LEN;
        str = malloc(len + 1);
        if (str == NULL) {
            cr_log_error("Internal error: Not enough memory\n");
            return;
        }
        str[len] = '\0';
        for (size_t i = 0; i < len; ++i)
            str[i] = ALPHANUM[rand() % (sizeof(ALPHANUM) - 1)];
        lvector_push_back(v, str);
    }
    str = NULL;
    lvector_foreach(pstr, v) {
        if (cr_asprintf(&str, "%s%s", str, *pstr) == -1) {
            cr_log_error("Internal error: string concatenation failed\n");
            return;
        }
    }
    TRY(str);
    free(str);
    lvector_destroy(v);
}