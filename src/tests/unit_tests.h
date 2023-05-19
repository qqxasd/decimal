#ifndef SRC_TESTS_TESTS_H_
#define SRC_TESTS_TESTS_H_

#include <check.h>

#include "../s21_decimal.h"

#define DEFAULT_COLOR = \033[0m
#define WHITE_COLOR = \033[97m
#define DARK_RED_COLOR = \033[31m
#define DARK_GREEN_COLOR = \033[32m
#define DARK_ORANGE_COLOR = \033[33m
#define DARK_BLUE_COLOR = \033[34m
#define DARK_PURPLE_COLOR = \033[35m
#define DARK_CYAN_COLOR = \033[36m
#define RED_COLOR = \033[91m
#define GREEN_COLOR = \033[92m
#define ORANGE_COLOR = \033[93m
#define BLUE_COLOR = \033[94m
#define PURPLE_COLOR = \033[95m
#define CYAN_COLOR = \033[96m

#define S21_TRUE 1
#define S21_FALSE 0
#define S21_MAX_U_INT 4294967295  // 0b11111111111111111111111111111111

Suite *suite_is_greater(void);
Suite *suite_is_equal(void);
Suite *suite_is_greater_or_equal(void);
Suite *suite_is_not_equal(void);
Suite *suite_is_less(void);
Suite *suite_is_less_or_equal(void);

void run_tests(void);
void run_testcase(Suite *testcase);

#endif  // SRC_TESTS_TESTS_H_