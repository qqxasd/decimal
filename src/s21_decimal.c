#include "s21_decimal.h"

int main() {
  s21_decimal decimal1, decimal2, result, result2;
  decimal1 = (s21_decimal){{0x0000000D, 0x00000000, 0x00000000, 0x000F0000}};
  decimal2 = (s21_decimal){{0x0000000A, 0x00000000, 0x00000000, 0x00130000}};
  s21_add(decimal1, decimal2, &result);
  printf("%d%d%d*10^(-%d)\n", result.bits[2], result.bits[1], result.bits[0],
         get_exp(result));
  s21_sub(decimal1, decimal2, &result2);
  printf("%d%d%d*10^(-%d)\n", result2.bits[2], result2.bits[1], result2.bits[0],
         get_exp(result2));
  return 0;
}

int setBit(int num, int pos) { return (num | (1 << pos)); }

int clearBit(int num, int pos) { return (num & (~(1 << pos))); }

int toggleBit(int num, int pos) { return (num ^ (1 << pos)); }

int get_exp(s21_decimal value) { return (value.bits[3] << 10) >> 26; }
int get_sign(s21_decimal value) { return (value.bits[3] >> 31) & 1; }
void equal_exponents(int exp_lower, int exp_higher,
                     s21_decimal* value_to_increase) {
  int diff = exp_higher - exp_lower;
  for (int i = 0; i < diff; i++) {
    (*value_to_increase).bits[0] *= 10;
    (*value_to_increase).bits[3] += 65536;
  }
}

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  int exp1 = get_exp(value_1);
  int exp2 = get_exp(value_2);
  if (exp1 > exp2) {
    equal_exponents(exp2, exp1, &value_2);
  } else if (exp2 > exp1) {
    equal_exponents(exp1, exp2, &value_1);
  }
  int carry_flag = 0;
  (*result).bits[0] = value_1.bits[0] + value_2.bits[0];
  (*result).bits[1] = value_1.bits[1] + value_2.bits[1];
  (*result).bits[2] = value_1.bits[2] + value_2.bits[2];
  (*result).bits[3] = value_2.bits[3];
  return exp1;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  int exp1 = get_exp(value_1);
  int exp2 = get_exp(value_2);
  if (exp1 > exp2) {
    equal_exponents(exp2, exp1, &value_2);
  } else if (exp2 > exp1) {
    equal_exponents(exp1, exp2, &value_1);
  }
  int carry_flag = 0;
  (*result).bits[0] = value_1.bits[0] - value_2.bits[0];
  (*result).bits[1] = value_1.bits[1] - value_2.bits[1];
  (*result).bits[2] = value_1.bits[2] - value_2.bits[2];
  (*result).bits[3] = value_2.bits[3];
  return exp1;
}