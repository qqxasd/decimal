#include <check.h>

#include "unit_tests.h"

START_TEST(s21_is_greater_test_1a) {
  s21_decimal a = {{2, 0, 0, 0}};
  s21_decimal b = {{1, 0, 0, 0}};
  ck_assert_int_eq(s21_is_greater(a, b), S21_TRUE);
}
END_TEST

START_TEST(s21_is_greater_test_2a) {
  s21_decimal a = {{S21_MAX_U_INT, 0, 0, 0}};
  s21_decimal b = {{0, 0, 0, 0}};
  printf("\n***\n%i, %i, %i, %i\n%i, %i, %i, %i\n***\n", a.bits[0], a.bits[1],
         a.bits[2], a.bits[3], b.bits[0], b.bits[1], b.bits[2], b.bits[3]);

  ck_assert_int_eq(s21_is_greater(a, b), S21_TRUE);
}
END_TEST

START_TEST(s21_is_greater_test_2_1a) {
  s21_decimal a = {{S21_MAX_U_INT, 0, 0, 0}};
  s21_decimal b = {{S21_MAX_U_INT - 1, 0, 0, 0}};
  ck_assert_int_eq(s21_is_greater(a, b), S21_TRUE);
}
END_TEST

START_TEST(s21_is_greater_test_3a) {
  s21_decimal a = {{S21_MAX_U_INT, 0, 0, 0}};
  s21_decimal b = {{S21_MAX_U_INT, 0, 0, 0}};
  ck_assert_int_eq(s21_is_greater(a, b), S21_FALSE);
}
END_TEST

START_TEST(s21_is_greater_test_4a) {
  s21_decimal a = {{S21_MAX_U_INT, 0, 0, 0}};
  s21_decimal b = {{S21_MAX_U_INT, 1, 0, 0}};
  ck_assert_int_eq(s21_is_greater(a, b), S21_FALSE);
}
END_TEST

START_TEST(s21_is_greater_test_4_1a) {
  s21_decimal a = {{S21_MAX_U_INT, 1, 0, 0}};
  s21_decimal b = {{S21_MAX_U_INT, 0, 0, 0}};
  ck_assert_int_eq(s21_is_greater(a, b), S21_TRUE);
}
END_TEST

START_TEST(s21_is_greater_test_5a) {
  s21_decimal a = {{S21_MAX_U_INT, S21_MAX_U_INT, 0, 0}};
  s21_decimal b = {{S21_MAX_U_INT, S21_MAX_U_INT - 1, 0, 0}};
  ck_assert_int_eq(s21_is_greater(a, b), S21_TRUE);
}
END_TEST

START_TEST(s21_is_greater_test_5_1a) {
  s21_decimal a = {{S21_MAX_U_INT, S21_MAX_U_INT, 0, 0}};
  s21_decimal b = {{S21_MAX_U_INT, S21_MAX_U_INT, 0, 0}};
  ck_assert_int_eq(s21_is_greater(a, b), S21_FALSE);
}
END_TEST

START_TEST(s21_is_greater_test_6a) {
  s21_decimal a = {{S21_MAX_U_INT, S21_MAX_U_INT, S21_MAX_U_INT, 0}};
  s21_decimal b = {{S21_MAX_U_INT, S21_MAX_U_INT, S21_MAX_U_INT - 1, 0}};
  ck_assert_int_eq(s21_is_greater(a, b), S21_TRUE);
}
END_TEST

START_TEST(s21_is_greater_test_7a) {
  s21_decimal a = {{S21_MAX_U_INT, S21_MAX_U_INT, S21_MAX_U_INT - 1, 0}};
  s21_decimal b = {{S21_MAX_U_INT, S21_MAX_U_INT, 0, 0}};
  ck_assert_int_eq(s21_is_greater(a, b), S21_TRUE);
}
END_TEST

START_TEST(s21_is_greater_test_8a) {
  s21_decimal a = {{0, S21_MAX_U_INT, S21_MAX_U_INT, 0}};
  s21_decimal b = {{0, S21_MAX_U_INT, S21_MAX_U_INT - 1, 0}};
  ck_assert_int_eq(s21_is_greater(a, b), S21_TRUE);
}
END_TEST

START_TEST(s21_is_greater_test_9a) {
  s21_decimal a = {{0, 0, S21_MAX_U_INT, 0}};
  s21_decimal b = {{0, 0, S21_MAX_U_INT - 1, 0}};
  ck_assert_int_eq(s21_is_greater(a, b), S21_TRUE);
}
END_TEST

START_TEST(s21_is_greater_test_10a) {
  s21_decimal a = {{0, 0, 0, 0}};
  s21_decimal b = {{0, 0, S21_MAX_U_INT, 0}};
  ck_assert_int_eq(s21_is_greater(a, b), S21_FALSE);
}
END_TEST

// START_TEST(s21_is_greater_test_1b) {
//   float num1 = 1.375342323523;
//   float num2 = 1.39;
//   s21_decimal dec1, dec2;
//   s21_from_float_to_decimal(num1, &dec1);
//   s21_from_float_to_decimal(num2, &dec2);
//   int res = s21_is_greater(dec1, dec2);
//   ck_assert_int_eq(res, S21_FALSE);
// }
// END_TEST

// START_TEST(s21_is_greater_test_2b) {
//   float num1 = 1.39;
//   float num2 = 1.39;
//   s21_decimal dec1, dec2;
//   s21_from_float_to_decimal(num1, &dec1);
//   s21_from_float_to_decimal(num2, &dec2);
//   int res = s21_is_greater(dec1, dec2);
//   ck_assert_int_eq(res, S21_FALSE);
// }
// END_TEST

// START_TEST(s21_is_greater_test_3b) {
//   float num1 = 1.39;
//   float num2 = -1.39;
//   s21_decimal dec1, dec2;
//   s21_from_float_to_decimal(num1, &dec1);
//   s21_from_float_to_decimal(num2, &dec2);
//   int res = s21_is_greater(dec1, dec2);
//   ck_assert_int_eq(res, S21_TRUE);
// }
// END_TEST

// START_TEST(s21_is_greater_test_4b) {
//   int num1 = 0;
//   int num2 = 0;
//   s21_decimal dec1, dec2;
//   s21_from_int_to_decimal(num1, &dec1);
//   s21_from_int_to_decimal(num2, &dec2);
//   int res = s21_is_greater(dec1, dec2);
//   ck_assert_int_eq(res, S21_FALSE);
// }
// END_TEST

// START_TEST(s21_is_greater_test_5b) {
//   int num1 = 3;
//   int num2 = 9;
//   s21_decimal dec1, dec2;
//   s21_from_int_to_decimal(num1, &dec1);
//   s21_from_int_to_decimal(num2, &dec2);
//   int res = s21_is_greater(dec1, dec2);
//   ck_assert_int_eq(res, S21_FALSE);
// }
// END_TEST

// START_TEST(s21_is_greater_test_6b) {
//   int num1 = -3;
//   int num2 = -3;
//   s21_decimal dec1, dec2;
//   s21_from_int_to_decimal(num1, &dec1);
//   s21_from_int_to_decimal(num2, &dec2);
//   int res = s21_is_greater(dec1, dec2);
//   ck_assert_int_eq(res, S21_FALSE);
// }
// END_TEST

// START_TEST(s21_is_greater_test_7b) {
//   int num1 = -3;
//   int num2 = 3;
//   s21_decimal dec1, dec2;
//   s21_from_int_to_decimal(num1, &dec1);
//   s21_from_int_to_decimal(num2, &dec2);
//   int res = s21_is_greater(dec1, dec2);
//   ck_assert_int_eq(res, S21_FALSE);
// }
// END_TEST

// START_TEST(s21_is_greater_test_8b) {
//   float num1 = -3.232446546;
//   float num2 = -3.2;
//   s21_decimal dec1, dec2;
//   s21_from_float_to_decimal(num1, &dec1);
//   s21_from_float_to_decimal(num2, &dec2);
//   int res = s21_is_greater(dec1, dec2);
//   ck_assert_int_eq(res, S21_FALSE);
// }
// END_TEST

// START_TEST(s21_is_greater_test_9b) {
//   float num1 = -345.232446546;
//   float num2 = -3.2;
//   s21_decimal dec1, dec2;
//   s21_from_float_to_decimal(num1, &dec1);
//   s21_from_float_to_decimal(num2, &dec2);
//   int res = s21_is_greater(dec1, dec2);
//   ck_assert_int_eq(res, S21_FALSE);
// }
// END_TEST

// START_TEST(s21_is_greater_test_10b) {
//   float num1 = -3.232323233232323233;
//   float num2 = -34545124.232446543232446543;
//   s21_decimal dec1, dec2;
//   s21_from_float_to_decimal(num1, &dec1);
//   s21_from_float_to_decimal(num2, &dec2);
//   int res = s21_is_greater(dec1, dec2);
//   ck_assert_int_eq(res, 1);
// }
// END_TEST

START_TEST(s21_is_greater_test_11b) {
  s21_decimal dec5 = {
      {12345, 0, 0, 0b00000000000001000000000000000000}};  //  1.2345
  s21_decimal dec6 = {
      {12, 0, 0, 0b10000000000000010000000000000000}};  //  -1.2;
  ck_assert_int_eq(s21_is_greater(dec5, dec6), 1);
  ck_assert_int_eq(s21_is_greater(dec6, dec5), 0);

  s21_decimal dec7 = {
      {12345, 0, 0, 0b10000000000001000000000000000000}};  // -1.2345
  s21_decimal dec8 = {{12, 0, 0, 0b00000000000000010000000000000000}};
  //  1.2; ck_assert_int_eq(s21_is_greater(dec7, dec8), 0);
  ck_assert_int_eq(s21_is_greater(dec8, dec7), 1);

  s21_decimal dec1 = {
      {12345, 0, 0, 0b00000000000001000000000000000000}};  //  1.2345
  s21_decimal dec2 = {{12, 0, 0, 0b00000000000000010000000000000000}};
  //  1.2 ck_assert_int_eq(s21_is_greater(dec1, dec2), 1);
  ck_assert_int_eq(s21_is_greater(dec2, dec1), 0);

  s21_decimal dec3 = {
      {12345, 0, 0, 0b10000000000001000000000000000000}};  // -1.2345
  s21_decimal dec4 = {
      {12, 0, 0, 0b10000000000000010000000000000000}};  //   -1.2
  ck_assert_int_eq(s21_is_greater(dec3, dec4), 0);
  ck_assert_int_eq(s21_is_greater(dec4, dec3), 1);
}
END_TEST

START_TEST(s21_is_greater_test_1) {
  // 3820
  s21_decimal a, b;
  // src1 = 2;
  // src2 = 3;

  a.bits[0] = 0b00000000000000000000000000000010;
  a.bits[1] = 0b00000000000000000000000000000000;
  a.bits[2] = 0b00000000000000000000000000000000;
  a.bits[3] = 0b00000000000000000000000000000000;
  b.bits[0] = 0b00000000000000000000000000000011;
  b.bits[1] = 0b00000000000000000000000000000000;
  b.bits[2] = 0b00000000000000000000000000000000;
  b.bits[3] = 0b00000000000000000000000000000000;
  int result = s21_is_greater(a, b);
  int origin = 0;
  ck_assert_int_eq(result, origin);
}
END_TEST

START_TEST(s21_is_greater_test_2) {
  // 3838
  s21_decimal a, b;
  // src1 = 3;
  // src2 = 2;

  a.bits[0] = 0b00000000000000000000000000000011;
  a.bits[1] = 0b00000000000000000000000000000000;
  a.bits[2] = 0b00000000000000000000000000000000;
  a.bits[3] = 0b00000000000000000000000000000000;
  b.bits[0] = 0b00000000000000000000000000000010;
  b.bits[1] = 0b00000000000000000000000000000000;
  b.bits[2] = 0b00000000000000000000000000000000;
  b.bits[3] = 0b00000000000000000000000000000000;
  int result = s21_is_greater(a, b);
  int origin = 1;
  ck_assert_int_eq(result, origin);
}
END_TEST

START_TEST(s21_is_greater_test_3) {
  // 3856
  s21_decimal a, b;
  // src1 = 3;
  // src2 = 3;

  a.bits[0] = 0b00000000000000000000000000000011;
  a.bits[1] = 0b00000000000000000000000000000000;
  a.bits[2] = 0b00000000000000000000000000000000;
  a.bits[3] = 0b00000000000000000000000000000000;
  b.bits[0] = 0b00000000000000000000000000000011;
  b.bits[1] = 0b00000000000000000000000000000000;
  b.bits[2] = 0b00000000000000000000000000000000;
  b.bits[3] = 0b00000000000000000000000000000000;
  int result = s21_is_greater(a, b);
  int origin = 0;
  ck_assert_int_eq(result, origin);
}
END_TEST

START_TEST(s21_is_greater_test_4) {
  // 3874
  s21_decimal a, b;
  // src1 = 2;
  // src2 = 0;

  a.bits[0] = 0b00000000000000000000000000000010;
  a.bits[1] = 0b00000000000000000000000000000000;
  a.bits[2] = 0b00000000000000000000000000000000;
  a.bits[3] = 0b00000000000000000000000000000000;
  b.bits[0] = 0b00000000000000000000000000000000;
  b.bits[1] = 0b00000000000000000000000000000000;
  b.bits[2] = 0b00000000000000000000000000000000;
  b.bits[3] = 0b00000000000000000000000000000000;
  int result = s21_is_greater(a, b);
  int origin = 1;
  ck_assert_int_eq(result, origin);
}
END_TEST

START_TEST(s21_is_greater_test_5) {
  // 3892
  s21_decimal a, b;
  // src1 = 0;
  // src2 = 3;

  a.bits[0] = 0b00000000000000000000000000000000;
  a.bits[1] = 0b00000000000000000000000000000000;
  a.bits[2] = 0b00000000000000000000000000000000;
  a.bits[3] = 0b00000000000000000000000000000000;
  b.bits[0] = 0b00000000000000000000000000000011;
  b.bits[1] = 0b00000000000000000000000000000000;
  b.bits[2] = 0b00000000000000000000000000000000;
  b.bits[3] = 0b00000000000000000000000000000000;
  int result = s21_is_greater(a, b);
  int origin = 0;
  ck_assert_int_eq(result, origin);
}
END_TEST

START_TEST(s21_is_greater_test_6) {
  // 3910
  s21_decimal a, b;
  // src1 = 2;
  // src2 = 0;

  a.bits[0] = 0b00000000000000000000000000000010;
  a.bits[1] = 0b00000000000000000000000000000000;
  a.bits[2] = 0b00000000000000000000000000000000;
  a.bits[3] = 0b00000000000000000000000000000000;
  b.bits[0] = 0b00000000000000000000000000000000;
  b.bits[1] = 0b00000000000000000000000000000000;
  b.bits[2] = 0b00000000000000000000000000000000;
  b.bits[3] = 0b00000000000000000000000000000000;
  int result = s21_is_greater(a, b);
  int origin = 1;
  ck_assert_int_eq(result, origin);
}
END_TEST

START_TEST(s21_is_greater_test_7) {
  // 3928
  s21_decimal a, b;
  // src1 = 6521;
  // src2 = 74121;

  a.bits[0] = 0b00000000000000000001100101111001;
  a.bits[1] = 0b00000000000000000000000000000000;
  a.bits[2] = 0b00000000000000000000000000000000;
  a.bits[3] = 0b00000000000000000000000000000000;
  b.bits[0] = 0b00000000000000010010000110001001;
  b.bits[1] = 0b00000000000000000000000000000000;
  b.bits[2] = 0b00000000000000000000000000000000;
  b.bits[3] = 0b00000000000000000000000000000000;
  int result = s21_is_greater(a, b);
  int origin = 0;
  ck_assert_int_eq(result, origin);
}
END_TEST

START_TEST(s21_is_greater_test_8) {
  // 3946
  s21_decimal a, b;
  // src1 = 4;
  // src2 = 97623323;

  a.bits[0] = 0b00000000000000000000000000000100;
  a.bits[1] = 0b00000000000000000000000000000000;
  a.bits[2] = 0b00000000000000000000000000000000;
  a.bits[3] = 0b00000000000000000000000000000000;
  b.bits[0] = 0b00000101110100011001110100011011;
  b.bits[1] = 0b00000000000000000000000000000000;
  b.bits[2] = 0b00000000000000000000000000000000;
  b.bits[3] = 0b00000000000000000000000000000000;
  int result = s21_is_greater(a, b);
  int origin = 0;
  ck_assert_int_eq(result, origin);
}
END_TEST

START_TEST(s21_is_greater_test_9) {
  // 3964
  s21_decimal a, b;
  // src1 = 65658654;
  // src2 = 5;

  a.bits[0] = 0b00000011111010011101111100011110;
  a.bits[1] = 0b00000000000000000000000000000000;
  a.bits[2] = 0b00000000000000000000000000000000;
  a.bits[3] = 0b00000000000000000000000000000000;
  b.bits[0] = 0b00000000000000000000000000000101;
  b.bits[1] = 0b00000000000000000000000000000000;
  b.bits[2] = 0b00000000000000000000000000000000;
  b.bits[3] = 0b00000000000000000000000000000000;
  int result = s21_is_greater(a, b);
  int origin = 1;
  ck_assert_int_eq(result, origin);
}
END_TEST

START_TEST(s21_is_greater_test_10) {
  // 3982
  s21_decimal a, b;
  // src1 = -364748;
  // src2 = 1;

  a.bits[0] = 0b00000000000001011001000011001100;
  a.bits[1] = 0b00000000000000000000000000000000;
  a.bits[2] = 0b00000000000000000000000000000000;
  a.bits[3] = 0b10000000000000000000000000000000;
  b.bits[0] = 0b00000000000000000000000000000001;
  b.bits[1] = 0b00000000000000000000000000000000;
  b.bits[2] = 0b00000000000000000000000000000000;
  b.bits[3] = 0b00000000000000000000000000000000;
  int result = s21_is_greater(a, b);
  int origin = 0;
  ck_assert_int_eq(result, origin);
}
END_TEST

START_TEST(s21_is_greater_test_11) {
  // 4000
  s21_decimal a, b;
  // src1 = 1;
  // src2 = 98745654321;

  a.bits[0] = 0b00000000000000000000000000000001;
  a.bits[1] = 0b00000000000000000000000000000000;
  a.bits[2] = 0b00000000000000000000000000000000;
  a.bits[3] = 0b00000000000000000000000000000000;
  b.bits[0] = 0b11111101101100110001110000110001;
  b.bits[1] = 0b00000000000000000000000000010110;
  b.bits[2] = 0b00000000000000000000000000000000;
  b.bits[3] = 0b00000000000000000000000000000000;
  int result = s21_is_greater(a, b);
  int origin = 0;
  ck_assert_int_eq(result, origin);
}
END_TEST

START_TEST(s21_is_greater_test_12) {
  // 4018
  s21_decimal a, b;
  // src1 = -9878798789;
  // src2 = -3;

  a.bits[0] = 0b01001100110100101000000111000101;
  a.bits[1] = 0b00000000000000000000000000000010;
  a.bits[2] = 0b00000000000000000000000000000000;
  a.bits[3] = 0b10000000000000000000000000000000;
  b.bits[0] = 0b00000000000000000000000000000011;
  b.bits[1] = 0b00000000000000000000000000000000;
  b.bits[2] = 0b00000000000000000000000000000000;
  b.bits[3] = 0b10000000000000000000000000000000;
  int result = s21_is_greater(a, b);
  int origin = 0;
  ck_assert_int_eq(result, origin);
}
END_TEST

START_TEST(s21_is_greater_test_13) {
  // 4036
  s21_decimal a, b;
  // src1 = 9999999999999999999;
  // src2 = 1;

  a.bits[0] = 0b10001001111001111111111111111111;
  a.bits[1] = 0b10001010110001110010001100000100;
  a.bits[2] = 0b00000000000000000000000000000000;
  a.bits[3] = 0b00000000000000000000000000000000;
  b.bits[0] = 0b00000000000000000000000000000001;
  b.bits[1] = 0b00000000000000000000000000000000;
  b.bits[2] = 0b00000000000000000000000000000000;
  b.bits[3] = 0b00000000000000000000000000000000;
  int result = s21_is_greater(a, b);
  int origin = 1;
  ck_assert_int_eq(result, origin);
}
END_TEST

START_TEST(s21_is_greater_test_14) {
  // 4054
  s21_decimal a, b;
  // src1 = 18446744073709551615;
  // src2 = 1;

  a.bits[0] = 0b11111111111111111111111111111111;
  a.bits[1] = 0b11111111111111111111111111111111;
  a.bits[2] = 0b00000000000000000000000000000000;
  a.bits[3] = 0b00000000000000000000000000000000;
  b.bits[0] = 0b00000000000000000000000000000001;
  b.bits[1] = 0b00000000000000000000000000000000;
  b.bits[2] = 0b00000000000000000000000000000000;
  b.bits[3] = 0b00000000000000000000000000000000;
  int result = s21_is_greater(a, b);
  int origin = 1;
  ck_assert_int_eq(result, origin);
}
END_TEST

START_TEST(s21_is_greater_test_15) {
  // 4072
  s21_decimal a, b;
  // src1 = 18446744073709551615.0;
  // src2 = 965453154;

  a.bits[0] = 0b11111111111111111111111111110110;
  a.bits[1] = 0b11111111111111111111111111111111;
  a.bits[2] = 0b00000000000000000000000000001001;
  a.bits[3] = 0b00000000000000010000000000000000;
  b.bits[0] = 0b00111001100010111010010101100010;
  b.bits[1] = 0b00000000000000000000000000000000;
  b.bits[2] = 0b00000000000000000000000000000000;
  b.bits[3] = 0b00000000000000000000000000000000;
  int result = s21_is_greater(a, b);
  int origin = 1;
  ck_assert_int_eq(result, origin);
}
END_TEST

START_TEST(s21_is_greater_test_16) {
  // 4090
  s21_decimal a, b;
  // src1 = -545454512454545.35265454545645;
  // src2 = 54564654;

  a.bits[0] = 0b10000010111000100101101011101101;
  a.bits[1] = 0b11111001111010000010010110101101;
  a.bits[2] = 0b10110000001111101111000010010100;
  a.bits[3] = 0b10000000000011100000000000000000;
  b.bits[0] = 0b00000011010000001001011100101110;
  b.bits[1] = 0b00000000000000000000000000000000;
  b.bits[2] = 0b00000000000000000000000000000000;
  b.bits[3] = 0b00000000000000000000000000000000;
  int result = s21_is_greater(a, b);
  int origin = 0;
  ck_assert_int_eq(result, origin);
}
END_TEST

START_TEST(s21_is_greater_test_17) {
  // 4108
  s21_decimal a, b;
  // src1 = -545454512454545.35265454545645;
  // src2 = -5.352654545456454545645464;

  a.bits[0] = 0b10000010111000100101101011101101;
  a.bits[1] = 0b11111001111010000010010110101101;
  a.bits[2] = 0b10110000001111101111000010010100;
  a.bits[3] = 0b10000000000011100000000000000000;
  b.bits[0] = 0b01001000000110110001111110011000;
  b.bits[1] = 0b11111011111111011000100101101101;
  b.bits[2] = 0b00000000000001000110110101110111;
  b.bits[3] = 0b10000000000110000000000000000000;
  int result = s21_is_greater(a, b);
  int origin = 0;
  ck_assert_int_eq(result, origin);
}
END_TEST

START_TEST(s21_is_greater_test_18) {
  // 4126
  s21_decimal a, b;
  // src1 = 7961327845421.879754123131254;
  // src2 = 0;

  a.bits[0] = 0b01001110111001000011100101110110;
  a.bits[1] = 0b01001011001101011010000111011001;
  a.bits[2] = 0b00011001101110010111010010111111;
  a.bits[3] = 0b00000000000011110000000000000000;
  b.bits[0] = 0b00000000000000000000000000000000;
  b.bits[1] = 0b00000000000000000000000000000000;
  b.bits[2] = 0b00000000000000000000000000000000;
  b.bits[3] = 0b00000000000000000000000000000000;
  int result = s21_is_greater(a, b);
  int origin = 1;
  ck_assert_int_eq(result, origin);
}
END_TEST

START_TEST(s21_is_greater_test_19) {
  // 4144
  s21_decimal a, b;
  // src1 = 12345677.987654345678987654346;
  // src2 = 87654323456.9876545678987653;

  a.bits[0] = 0b10010001000010101111010011001010;
  a.bits[1] = 0b11000000010001011101010111110010;
  a.bits[2] = 0b00100111111001000001101100000000;
  a.bits[3] = 0b00000000000101010000000000000000;
  b.bits[0] = 0b00010001110011011101000110000101;
  b.bits[1] = 0b11110101101111000110111111000000;
  b.bits[2] = 0b00000010110101010000111100111111;
  b.bits[3] = 0b00000000000100000000000000000000;
  int result = s21_is_greater(a, b);
  int origin = 0;
  ck_assert_int_eq(result, origin);
}
END_TEST

START_TEST(s21_is_greater_test_20) {
  // 4162
  s21_decimal a, b;
  // src1 = -12345677.987654345678987654346;
  // src2 = 87654323456.9876545678987653;

  a.bits[0] = 0b10010001000010101111010011001010;
  a.bits[1] = 0b11000000010001011101010111110010;
  a.bits[2] = 0b00100111111001000001101100000000;
  a.bits[3] = 0b10000000000101010000000000000000;
  b.bits[0] = 0b00010001110011011101000110000101;
  b.bits[1] = 0b11110101101111000110111111000000;
  b.bits[2] = 0b00000010110101010000111100111111;
  b.bits[3] = 0b00000000000100000000000000000000;
  int result = s21_is_greater(a, b);
  int origin = 0;
  ck_assert_int_eq(result, origin);
}
END_TEST

START_TEST(s21_is_greater_test_21) {
  // 4180
  s21_decimal a, b;
  // src1 = 12345677.987654345678987654346;
  // src2 = -87654323456.9876545678987653;

  a.bits[0] = 0b10010001000010101111010011001010;
  a.bits[1] = 0b11000000010001011101010111110010;
  a.bits[2] = 0b00100111111001000001101100000000;
  a.bits[3] = 0b00000000000101010000000000000000;
  b.bits[0] = 0b00010001110011011101000110000101;
  b.bits[1] = 0b11110101101111000110111111000000;
  b.bits[2] = 0b00000010110101010000111100111111;
  b.bits[3] = 0b10000000000100000000000000000000;
  int result = s21_is_greater(a, b);
  int origin = 1;
  ck_assert_int_eq(result, origin);
}
END_TEST

START_TEST(s21_is_greater_test_22) {
  // 4198
  s21_decimal a, b;
  // src1 = -12345677.987654345678987654346;
  // src2 = -87654323456.9876545678987653;

  a.bits[0] = 0b10010001000010101111010011001010;
  a.bits[1] = 0b11000000010001011101010111110010;
  a.bits[2] = 0b00100111111001000001101100000000;
  a.bits[3] = 0b10000000000101010000000000000000;
  b.bits[0] = 0b00010001110011011101000110000101;
  b.bits[1] = 0b11110101101111000110111111000000;
  b.bits[2] = 0b00000010110101010000111100111111;
  b.bits[3] = 0b10000000000100000000000000000000;
  int result = s21_is_greater(a, b);
  int origin = 1;
  ck_assert_int_eq(result, origin);
}
END_TEST

START_TEST(s21_is_greater_test_23) {
  // 4216
  s21_decimal a, b;
  // src1 = 0;
  // src2 = 0;

  a.bits[0] = 0b00000000000000000000000000000000;
  a.bits[1] = 0b00000000000000000000000000000000;
  a.bits[2] = 0b00000000000000000000000000000000;
  a.bits[3] = 0b00000000000000000000000000000000;
  b.bits[0] = 0b00000000000000000000000000000000;
  b.bits[1] = 0b00000000000000000000000000000000;
  b.bits[2] = 0b00000000000000000000000000000000;
  b.bits[3] = 0b00000000000000000000000000000000;
  int result = s21_is_greater(a, b);
  int origin = 0;
  ck_assert_int_eq(result, origin);
}
END_TEST

START_TEST(s21_is_greater_test_24) {
  // 4234
  s21_decimal a, b;
  // src1 = 0;
  // src2 = 0;

  a.bits[0] = 0b00000000000000000000000000000000;
  a.bits[1] = 0b00000000000000000000000000000000;
  a.bits[2] = 0b00000000000000000000000000000000;
  a.bits[3] = 0b00000000000000000000000000000000;
  b.bits[0] = 0b00000000000000000000000000000000;
  b.bits[1] = 0b00000000000000000000000000000000;
  b.bits[2] = 0b00000000000000000000000000000000;
  b.bits[3] = 0b00000000000000000000000000000000;
  int result = s21_is_greater(a, b);
  int origin = 0;
  ck_assert_int_eq(result, origin);
}
END_TEST

START_TEST(s21_is_greater_test_25) {
  // 4252
  s21_decimal a, b;
  // src1 = 0;
  // src2 = 0;

  a.bits[0] = 0b00000000000000000000000000000000;
  a.bits[1] = 0b00000000000000000000000000000000;
  a.bits[2] = 0b00000000000000000000000000000000;
  a.bits[3] = 0b00000000000000000000000000000000;
  b.bits[0] = 0b00000000000000000000000000000000;
  b.bits[1] = 0b00000000000000000000000000000000;
  b.bits[2] = 0b00000000000000000000000000000000;
  b.bits[3] = 0b00000000000000000000000000000000;
  int result = s21_is_greater(a, b);
  int origin = 0;
  ck_assert_int_eq(result, origin);
}
END_TEST

START_TEST(s21_is_greater_test_26) {
  // 4270
  s21_decimal a, b;
  // src1 = 79228162514264337593543950335;
  // src2 = -1;

  a.bits[0] = 0b11111111111111111111111111111111;
  a.bits[1] = 0b11111111111111111111111111111111;
  a.bits[2] = 0b11111111111111111111111111111111;
  a.bits[3] = 0b00000000000000000000000000000000;
  b.bits[0] = 0b00000000000000000000000000000001;
  b.bits[1] = 0b00000000000000000000000000000000;
  b.bits[2] = 0b00000000000000000000000000000000;
  b.bits[3] = 0b10000000000000000000000000000000;
  int result = s21_is_greater(a, b);
  int origin = 1;
  ck_assert_int_eq(result, origin);
}
END_TEST

START_TEST(s21_is_greater_test_27) {
  // 4288
  s21_decimal a, b;
  // src1 = 7922816251427554395;
  // src2 = 65645646;

  a.bits[0] = 0b01011111000010000000010001011011;
  a.bits[1] = 0b01101101111100110111111101100111;
  a.bits[2] = 0b00000000000000000000000000000000;
  a.bits[3] = 0b00000000000000000000000000000000;
  b.bits[0] = 0b00000011111010011010110001001110;
  b.bits[1] = 0b00000000000000000000000000000000;
  b.bits[2] = 0b00000000000000000000000000000000;
  b.bits[3] = 0b00000000000000000000000000000000;
  int result = s21_is_greater(a, b);
  int origin = 1;
  ck_assert_int_eq(result, origin);
}
END_TEST

START_TEST(s21_is_greater_test_28) {
  // 4306
  s21_decimal a, b;
  // src1 = 665464545;
  // src2 = 8798232189789785;

  a.bits[0] = 0b00100111101010100010111011100001;
  a.bits[1] = 0b00000000000000000000000000000000;
  a.bits[2] = 0b00000000000000000000000000000000;
  a.bits[3] = 0b00000000000000000000000000000000;
  b.bits[0] = 0b00010000010100101110011001011001;
  b.bits[1] = 0b00000000000111110100000111110010;
  b.bits[2] = 0b00000000000000000000000000000000;
  b.bits[3] = 0b00000000000000000000000000000000;
  int result = s21_is_greater(a, b);
  int origin = 0;
  ck_assert_int_eq(result, origin);
}
END_TEST

START_TEST(s21_is_greater_test_29) {
  // 4324
  s21_decimal a, b;
  // src1 = 2.7986531268974139743;
  // src2 = 9.979623121254565121244554;

  a.bits[0] = 0b11111110100100001101100101011111;
  a.bits[1] = 0b10000100011001000010000111101000;
  a.bits[2] = 0b00000000000000000000000000000001;
  a.bits[3] = 0b00000000000100110000000000000000;
  b.bits[0] = 0b11010100101011100011110110001010;
  b.bits[1] = 0b01110100000101000010011110100011;
  b.bits[2] = 0b00000000000010000100000101000100;
  b.bits[3] = 0b00000000000110000000000000000000;
  int result = s21_is_greater(a, b);
  int origin = 0;
  ck_assert_int_eq(result, origin);
}
END_TEST

START_TEST(s21_is_greater_test_30) {
  // 4342
  s21_decimal a, b;
  // src1 = -9798956154578676.797564534156;
  // src2 = -2156878451.854764;

  a.bits[0] = 0b01010100010000000011110110001100;
  a.bits[1] = 0b10001011010100100000010101011001;
  a.bits[2] = 0b00011111101010011000000110101101;
  a.bits[3] = 0b10000000000011000000000000000000;
  b.bits[0] = 0b01100101111100100100110110101100;
  b.bits[1] = 0b00000000000001111010100110101011;
  b.bits[2] = 0b00000000000000000000000000000000;
  b.bits[3] = 0b10000000000001100000000000000000;
  int result = s21_is_greater(a, b);
  int origin = 0;
  ck_assert_int_eq(result, origin);
}
END_TEST

START_TEST(s21_is_greater_test_31) {
  // 4360
  s21_decimal a, b;
  // src1 = 0.5456465465486476846545465485;
  // src2 = 0.68985125146545154;

  a.bits[0] = 0b00001011000011110111000010001101;
  a.bits[1] = 0b10010110101100000101100101010001;
  a.bits[2] = 0b00010001101000010111101110000001;
  a.bits[3] = 0b00000000000111000000000000000000;
  b.bits[0] = 0b00010111001001010100110000000010;
  b.bits[1] = 0b00000000111101010001010110011011;
  b.bits[2] = 0b00000000000000000000000000000000;
  b.bits[3] = 0b00000000000100010000000000000000;
  int result = s21_is_greater(a, b);
  int origin = 0;
  // printf("\n***\n%u, %u, %u, %u\n%u, %u, %u, %u\n***\n", a.bits[3],
  // a.bits[2],
  //        a.bits[1], a.bits[0], b.bits[3], b.bits[2], b.bits[1], b.bits[0]);

  ck_assert_int_eq(result, origin);
}
END_TEST

START_TEST(s21_is_greater_test_32) {
  // 4378
  s21_decimal a, b;
  // src1 = -0.77545545454546589781;
  // src2 = 87894515154546456456;

  a.bits[0] = 0b10101100001010000001100001010101;
  a.bits[1] = 0b00110100001010010001111010111100;
  a.bits[2] = 0b00000000000000000000000000000100;
  a.bits[3] = 0b10000000000101000000000000000000;
  b.bits[0] = 0b00010011111011011111011110001000;
  b.bits[1] = 0b11000011110010000000101111011001;
  b.bits[2] = 0b00000000000000000000000000000100;
  b.bits[3] = 0b00000000000000000000000000000000;
  int result = s21_is_greater(a, b);
  int origin = 0;
  ck_assert_int_eq(result, origin);
}
END_TEST

START_TEST(s21_is_greater_test_33) {
  // 4396
  s21_decimal a, b;
  // src1 = -79228162514264337593543950335;
  // src2 = 1;

  a.bits[0] = 0b11111111111111111111111111111111;
  a.bits[1] = 0b11111111111111111111111111111111;
  a.bits[2] = 0b11111111111111111111111111111111;
  a.bits[3] = 0b10000000000000000000000000000000;
  b.bits[0] = 0b00000000000000000000000000000001;
  b.bits[1] = 0b00000000000000000000000000000000;
  b.bits[2] = 0b00000000000000000000000000000000;
  b.bits[3] = 0b00000000000000000000000000000000;
  int result = s21_is_greater(a, b);
  int origin = 0;
  ck_assert_int_eq(result, origin);
}
END_TEST

START_TEST(s21_is_greater_test_34) {
  // 4414
  s21_decimal a, b;
  // src1 = 32323465788987654;
  // src2 = 67543278.89765424354657687;

  a.bits[0] = 0b01011000010000110001100100000110;
  a.bits[1] = 0b00000000011100101101011000000101;
  a.bits[2] = 0b00000000000000000000000000000000;
  a.bits[3] = 0b00000000000000000000000000000000;
  b.bits[0] = 0b10111110000100011000010110010111;
  b.bits[1] = 0b11011001001111010001001011000110;
  b.bits[2] = 0b00000000000001011001011001001000;
  b.bits[3] = 0b00000000000100010000000000000000;
  int result = s21_is_greater(a, b);
  int origin = 1;
  // printf("\n***\n%u, %u, %u, %u\n%u, %u, %u, %u\n***\n", a.bits[3],
  // a.bits[2],
  //        a.bits[1], a.bits[0], b.bits[3], b.bits[2], b.bits[1], b.bits[0]);

  ck_assert_int_eq(result, origin);
}
END_TEST

START_TEST(s21_is_greater_test_35) {
  // 4432
  s21_decimal a, b;
  // src1 = -784515454.7989898652154545652;
  // src2 = -579895323215489956.67897455465;

  a.bits[0] = 0b10110001001110110100010111110100;
  a.bits[1] = 0b00111000100000111010110010000001;
  a.bits[2] = 0b00011001010110010101110000011000;
  a.bits[3] = 0b10000000000100110000000000000000;
  b.bits[0] = 0b11100110001001011001001101101001;
  b.bits[1] = 0b00111000110110101110001010110100;
  b.bits[2] = 0b10111011010111111101000100011110;
  b.bits[3] = 0b10000000000010110000000000000000;
  int result = s21_is_greater(a, b);
  int origin = 1;
  ck_assert_int_eq(result, origin);
}
END_TEST

START_TEST(s21_is_greater_test_36) {
  // 4450
  s21_decimal a, b;
  // src1 = 0;
  // src2 = -37986322154988653.784354545765;

  a.bits[0] = 0b00000000000000000000000000000000;
  a.bits[1] = 0b00000000000000000000000000000000;
  a.bits[2] = 0b00000000000000000000000000000000;
  a.bits[3] = 0b00000000000000000000000000000000;
  b.bits[0] = 0b01001111101101000110000001100101;
  b.bits[1] = 0b01000010001101101011011001100001;
  b.bits[2] = 0b01111010101111011000110010011010;
  b.bits[3] = 0b10000000000011000000000000000000;
  int result = s21_is_greater(a, b);
  int origin = 1;
  ck_assert_int_eq(result, origin);
}
END_TEST

Suite *suite_is_greater(void) {
  Suite *suite = suite_create("is_greater");
  TCase *testcase = tcase_create("case_is_greater");

  tcase_add_test(testcase, s21_is_greater_test_1a);
  tcase_add_test(testcase, s21_is_greater_test_2a);
  tcase_add_test(testcase, s21_is_greater_test_2_1a);
  tcase_add_test(testcase, s21_is_greater_test_3a);
  tcase_add_test(testcase, s21_is_greater_test_4a);
  tcase_add_test(testcase, s21_is_greater_test_4_1a);
  tcase_add_test(testcase, s21_is_greater_test_5a);
  tcase_add_test(testcase, s21_is_greater_test_5_1a);
  tcase_add_test(testcase, s21_is_greater_test_6a);
  tcase_add_test(testcase, s21_is_greater_test_7a);
  tcase_add_test(testcase, s21_is_greater_test_8a);
  tcase_add_test(testcase, s21_is_greater_test_9a);
  tcase_add_test(testcase, s21_is_greater_test_10a);

  // tcase_add_test(testcase, s21_is_greater_test_1b);
  // tcase_add_test(testcase, s21_is_greater_test_2b);
  // tcase_add_test(testcase, s21_is_greater_test_3b);
  // tcase_add_test(testcase, s21_is_greater_test_4b);
  // tcase_add_test(testcase, s21_is_greater_test_5b);
  // tcase_add_test(testcase, s21_is_greater_test_6b);
  // tcase_add_test(testcase, s21_is_greater_test_7b);
  // tcase_add_test(testcase, s21_is_greater_test_8b);
  // tcase_add_test(testcase, s21_is_greater_test_9b);
  // tcase_add_test(testcase, s21_is_greater_test_10b);
  tcase_add_test(testcase, s21_is_greater_test_11b);

  tcase_add_test(testcase, s21_is_greater_test_1);
  tcase_add_test(testcase, s21_is_greater_test_2);
  tcase_add_test(testcase, s21_is_greater_test_3);
  tcase_add_test(testcase, s21_is_greater_test_4);
  tcase_add_test(testcase, s21_is_greater_test_5);
  tcase_add_test(testcase, s21_is_greater_test_6);
  tcase_add_test(testcase, s21_is_greater_test_7);
  tcase_add_test(testcase, s21_is_greater_test_8);
  tcase_add_test(testcase, s21_is_greater_test_8);
  tcase_add_test(testcase, s21_is_greater_test_9);
  tcase_add_test(testcase, s21_is_greater_test_10);
  tcase_add_test(testcase, s21_is_greater_test_11);
  tcase_add_test(testcase, s21_is_greater_test_12);
  tcase_add_test(testcase, s21_is_greater_test_13);
  tcase_add_test(testcase, s21_is_greater_test_14);
  tcase_add_test(testcase, s21_is_greater_test_15);
  tcase_add_test(testcase, s21_is_greater_test_16);
  tcase_add_test(testcase, s21_is_greater_test_17);
  tcase_add_test(testcase, s21_is_greater_test_18);
  tcase_add_test(testcase, s21_is_greater_test_19);
  tcase_add_test(testcase, s21_is_greater_test_20);
  tcase_add_test(testcase, s21_is_greater_test_21);
  tcase_add_test(testcase, s21_is_greater_test_22);
  tcase_add_test(testcase, s21_is_greater_test_23);
  tcase_add_test(testcase, s21_is_greater_test_24);
  tcase_add_test(testcase, s21_is_greater_test_25);
  tcase_add_test(testcase, s21_is_greater_test_26);
  tcase_add_test(testcase, s21_is_greater_test_27);
  tcase_add_test(testcase, s21_is_greater_test_28);
  tcase_add_test(testcase, s21_is_greater_test_29);
  tcase_add_test(testcase, s21_is_greater_test_30);
  tcase_add_test(testcase, s21_is_greater_test_31);
  tcase_add_test(testcase, s21_is_greater_test_32);
  tcase_add_test(testcase, s21_is_greater_test_33);
  tcase_add_test(testcase, s21_is_greater_test_34);
  tcase_add_test(testcase, s21_is_greater_test_35);
  tcase_add_test(testcase, s21_is_greater_test_36);

  suite_add_tcase(suite, testcase);
  return suite;
}