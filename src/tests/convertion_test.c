#include <check.h>

#include "../decimal.h"

decimal dec;

START_TEST(dec_int_test) {
  int dec_int_real, tmp;
  int dec_int_res_exp[] = {1,       222,      3333,   44444,   5555, -11,
                           -2222,   -332,     -4123,  -53412,  0,    123123,
                           4093890, -1234092, 999999, 90099909};
  for (int i = 0; i < (int)(sizeof(dec_int_res_exp) / sizeof(int)); i++) {
    from_int_to_decimal(dec_int_res_exp[i], &dec);
    from_decimal_to_int(dec, &dec_int_real);
    ck_assert_int_eq(dec_int_real, dec_int_res_exp[i]);
  }
  decimal dec1[] = {
      {{0, 0, 1, 0}}, {{0, 122, 212, 121}}, {{0x10000000, 0x10000000, 0, 0}}};
  for (int i = 0; i < (int)(sizeof(dec1) / sizeof(decimal)); i++) {
    dec_int_real = from_decimal_to_int(dec1[i], &tmp);
    ck_assert_int_eq(dec_int_real, 1);
  }
}
END_TEST

START_TEST(dec_float_test) {
  float dec_float_real;
  float dec_float_exp[] = {12.2,    123.562,   2123.123,
                           2342.12, 0.1231251, 2222.222};
  // normal
  for (int i = 0; i < (int)(sizeof(dec_float_exp) / sizeof(int)); i++) {
    from_float_to_decimal(dec_float_exp[i], &dec);
    from_decimal_to_float(dec, &dec_float_real);
    ck_assert_float_eq(dec_float_exp[i], dec_float_real);
  }
  // too small/big
  float floa1[] = {1e-29, INFINITY};
  for (int i = 0; i < (int)(sizeof(floa1) / sizeof(float)); i++) {
    ck_assert_int_eq(1, from_float_to_decimal(floa1[i], &dec));
  }
  // only 7 digits
  float inp[] = {12345678901234, 0.1234567890123, 9999999999,
                 55556666666,    2222.33333334,   777.6666};
  float exp[] = {1234567, 0.1234567, 1000000, 5555666, 2222.333, 777.6666};
  for (int i = 0; i < (int)(sizeof(inp) / sizeof(float)); i++) {
    from_float_to_decimal(inp[i], &dec);
    from_decimal_to_float(dec, &dec_float_real);
    ck_assert_float_eq(exp[i], dec_float_real);
  }
}
END_TEST

START_TEST(float_dec_to_int_test) {
  int real;
  float inp[] = {12.2, 12312.122, 1231.211, 123143.23442};
  int expec[] = {12, 12312, 1231, 123143};
  for (int i = 0; i < (int)(sizeof(inp) / sizeof(int)); i++) {
    from_float_to_decimal(inp[i], &dec);
    from_decimal_to_int(dec, &real);
    ck_assert_int_eq(real, expec[i]);
  }
}
END_TEST

Suite *dec_int_suit() {
  Suite *s = suite_create("dec_int_suit");
  TCase *tcore = tcase_create("core");
  tcase_add_test(tcore, dec_int_test);
  suite_add_tcase(s, tcore);
  return s;
}

Suite *dec_float_suit() {
  Suite *s = suite_create("dec_float_suit");
  TCase *tcore = tcase_create("core");
  tcase_add_test(tcore, dec_float_test);
  suite_add_tcase(s, tcore);
  return s;
}

Suite *dec_int_float_suit() {
  Suite *s = suite_create("dec_int_float_suit");
  TCase *tcore = tcase_create("core");
  tcase_add_test(tcore, float_dec_to_int_test);
  suite_add_tcase(s, tcore);
  return s;
}
