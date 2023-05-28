#include <check.h>
#include "../s21_decimal.h"

int s21_from_int_to_decimal(
    int src, s21_decimal *dst);  // Переводы Денис (двигается на блессе)
int s21_from_float_to_decimal(float src, s21_decimal *dst);  //
int s21_from_decimal_to_int(s21_decimal src, int *dst);      //
int s21_from_decimal_to_float(s21_decimal src, float *dst);  //

s21_decimal dec;

START_TEST(dec_int_test) {
    int dec_int_real;
    int dec_int_res_exp[] = {1, 222, 3333, 44444, 5555, -11, -2222, -332, -4123, -53412};
    for (int i = 0; i < sizeof(dec_int_res_exp) / sizeof(int); i++) {
        s21_from_int_to_decimal(dec_int_res_exp[i], &dec);
        s21_from_decimal_to_int(dec, &dec_int_real);
        ck_assert_int_eq(dec_int_real, dec_int_res_exp[i]);
    }
}END_TEST

Suite *dec_int_suit() {
    Suite *s = suite_create("dec_int_suit");
    TCase *tcore = tcase_create("core");
    tcase_add_test(tcore, dec_int_test);
    suite_add_tcase(s, tcore);
    return s;
}

int main() {
    Suite *s = dec_int_suit();
    SRunner *sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    return 0;
}