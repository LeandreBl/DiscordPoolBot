__CODE__

#include <criterion/criterion.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define TRY(str)                                     \
    char *__s__ = strdup(str);                              \
    cr_assert_str_eq(__FUNCTION_NAME__(__s__), __FUNCTION_NAME__dummy(__s__));\
    free(__s__);

#define CHARGE_LOAD __ITERATIONS__
#define MAX_LEN 3000

char *__FUNCTION_NAME__dummy(char *str)
{
	size_t len = strlen(str);
	char tmp;

	for (size_t i = 0; i < len / 2; ++i) {
		tmp = str[i];
		str[i] = str[len - i - 1];
		str[len - i - 1] = tmp;
	}
	return str;
}

Test(__TEST_NAME__, easy)
{
    TRY("salut");
}

Test(__TEST_NAME__, empty)
{
    TRY("");
}

Test(__TEST_NAME__, long_string)
{
    TRY("aaaaaaaaaaaaaaaaaaaaa5z3464351354351z35441er32g4r3.g15r3t41g35r4tg52r1g");
}

Test(__TEST_NAME__, escape)
{
    TRY("\n\r\1\0");
}

const char ALPHANUM[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

Test(__TEST_NAME__, charge)
{
    size_t len;
    char *str;

    srand(time(NULL));
    for (size_t i = 0; i < CHARGE_LOAD; ++i) {
        len = rand() % MAX_LEN;
        str = malloc(len + 1);
        if (str == NULL) {
            cr_log_error("Internal error: Not enough memory\n");
            return;
        }
        str[len] = '\0';
        for (size_t i = 0; i < len; ++i)
            str[i] = ALPHANUM[rand() % (sizeof(ALPHANUM) - 1)];
        TRY(str);
        free(str);
    }
}