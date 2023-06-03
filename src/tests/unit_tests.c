#include "unit_tests.h"

int main(void) {
  Suite *list_cases[] = {
      suite_is_greater(), suite_add(),          suite_sub(),
      suite_mul(),        suite_div(),          dec_int_suit(),
      dec_float_suit(),   dec_int_float_suit(), NULL};

  for (Suite **current_testcase = list_cases; *current_testcase != NULL;
       current_testcase++) {
    run_testcase(*current_testcase);
  }
  return 0;
}

void run_testcase(Suite *testcase) {
  SRunner *sr = srunner_create(testcase);
  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_NORMAL);
  srunner_free(sr);
}
