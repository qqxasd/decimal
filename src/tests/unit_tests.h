#ifndef SRC_TESTS_TESTS_H_
#define SRC_TESTS_TESTS_H_

#include <check.h>

#include "../decimal.h"

#define TRUE 1
#define FALSE 0
#define MAX_U_INT 4294967295  // 0b11111111111111111111111111111111

Suite *suite_add(void);
Suite *suite_div(void);
Suite *suite_sub(void);
Suite *suite_mul(void);
Suite *suite_is_greater(void);
Suite *suite_is_equal(void);
Suite *suite_is_greater_or_equal(void);
Suite *suite_is_not_equal(void);
Suite *suite_is_less(void);
Suite *suite_is_less_or_equal(void);
Suite *suite_from_decimal_to_float(void);
Suite *dec_int_suit();
Suite *dec_float_suit();
Suite *dec_int_float_suit();

Suite *suite_truncate();
Suite *suite_negate();
Suite *suite_round();
Suite *suite_floor();

void run_testcase(Suite *testcase);

#endif  // SRC_TESTS_TESTS_H_