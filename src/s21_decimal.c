#include "s21_decimal.h"

int main() {
  s21_decimal result;
  s21_add((s21_decimal){{0x1000000F, 0x10000000, 0x0000000F, 0x800E0000}},
          (s21_decimal){{0xF0000001, 0xF0000000, 0x00000001, 0x800F0000}},
          &result);
  printf("%d %d %d\n", result.bits[0], result.bits[1], result.bits[2]);
}

int setBit(unsigned int num, int pos) { return (num | (1 << pos)); }

int getBit(unsigned int num, int pos) { return ((num & (1 << pos)) >> pos); }

int clearBit(unsigned int num, int pos) { return (num & (~(1 << pos))); }

int toggleBit(unsigned int num, int pos) { return (num ^ (1 << pos)); }

int get_exp(s21_decimal value) { return (value.bits[3] << 10) >> 26; }
int get_sign(s21_decimal value) { return (value.bits[3] >> 31) & 1; }

void equal_exponents(s21_decimal *value_1, s21_decimal *value_2) {
  int exp1 = get_exp(*value_1);
  int exp2 = get_exp(*value_2);
  int diff = (exp2 > exp1) * (exp2 - exp1) + (exp1 > exp2) * (exp1 - exp2);
  while (diff) {
    if (exp2 > exp1) {
      if (!increase_exponent(value_1)) {
        decrease_exponent(value_2);
      }
    } else {
      if (!increase_exponent(value_2)) {
        decrease_exponent(value_1);
      }
    }
    diff--;
  }
}

int increase_exponent(s21_decimal *value) {
  int res = 1;
  s21_decimal temp;
  s21_decimal ten =
      (s21_decimal){{0x0000000A, 0x00000000, 0x00000000, 0x00000000}};
  if (!s21_mul(*value, ten, &temp)) {
    (*value).bits[3] | 1 << 15;
    *value = temp;
  } else {
    res = 0;
  }
  if ((*value).bits[3] > 28) {
    res = 0;
  }
  return res;
}

void decrease_exponent(s21_decimal *value) {
  int last = (*value).bits[0] % 10;
  int pre_last = (*value).bits[0] % 10 % 10;
  s21_decimal ten =
      (s21_decimal){{0x0000000A, 0x00000000, 0x00000000, 0x00000000}};
  s21_decimal one =
      (s21_decimal){{0x00000001, 0x00000000, 0x00000000, 0x00000000}};
  s21_del(*value, ten, value);
  if (get_sign(*value) && last) {  // banking rounding
    s21_sub(*value, one, value);
  } else if ((getBit(pre_last, 0) && (last > 4)) ||
             (!getBit(pre_last, 0) &&
              (last > 5))) {  // odd prelast  with 5...9 last or even prelast
                              // with 6...9 last
    s21_add(*value, one, value);
  }
}

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = 0;
  *result = (s21_decimal){{0x00000000, 0x00000000, 0x00000000, 0x00000000}};
  equal_exponents(&value_1, &value_2);
  int carry = 0;
  for (int i = 0; i < 96; i++) {
    int bit1 = getBit(value_1.bits[i / 32], i % 32);
    int bit2 = getBit(value_2.bits[i / 32], i % 32);
    int sum = bit1 + bit2 + carry;
    carry = getBit(sum, 1);
    if (getBit(sum, 0)) {
      (*result).bits[i / 32] = setBit((*result).bits[i / 32], i % 32);
    }
  }
  (*result).bits[3] = get_sign(value_1) << 31 | (get_exp(value_1) - carry)
                                                    << 15;
  return res;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = 0;
  *result = (s21_decimal){{0x00000000, 0x00000000, 0x00000000, 0x00000000}};
  equal_exponents(&value_1, &value_2);
  int carry = 0;
  for (int i = 0; i < 96; i++) {
    int bit1 = getBit(value_1.bits[i / 32], i % 32);
    int bit2 = getBit(value_2.bits[i / 32], i % 32);
    int sum;
    int current_bit = i;
    while ((sum = bit1 - bit2 + getBit(carry, i % 32)) < 0) {
      if (getBit(value_1.bits[current_bit / 32], current_bit % 32)) {
        clearBit(value_1.bits[current_bit / 32], current_bit % 32);
        setBit(carry, current_bit % 32);
      } else {
        current_bit++;
      }
    }
    clearBit(carry, current_bit % 32);
  }
  (*result).bits[3] = get_sign(value_1) << 31 | (get_exp(value_1) + carry)
                                                    << 15;
  return res;
}

void basic_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result,
               int *res) {}

void basic_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result,
               int *res, int *change_sign) {}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = 0;
  *result =
      (s21_decimal){{0x00000000, 0x00000000, 0x00000000,
                     (getBit(get_sign(value_1) + get_sign(value_2), 1)) << 31 |
                         (get_exp(value_1) + get_exp(value_2)) << 15}};
  for (int i = 0; i < 96; i++) {
    int bit1 = getBit(value_1.bits[i / 32], i % 32);
    if (bit1) {
      s21_decimal value_to_add = value_2;
      if (i > 31) {
        value_to_add.bits[0] = 0;
      }
      if (i > 63) {
        value_to_add.bits[1] = 0;
        value_to_add.bits[0] = (value_to_add.bits[2] >> (i % 32));
        if (!value_to_add.bits[0]) {
          value_to_add.bits[3] | (1 << 15);
        }
      }
      value_to_add.bits[i / 32] << (i % 32);
      s21_add(*result, value_to_add, result);
    }
  }
  return res;
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = 0;
  *result =
      (s21_decimal){{0x00000000, 0x00000000, 0x00000000,
                     (getBit(get_sign(value_1) + get_sign(value_2), 1)) << 31 |
                         (get_exp(value_1) - get_exp(value_2)) << 15}};
  int q = 0;
  s21_decimal tmp =
      (s21_decimal){{0x00000000, 0x00000000, 0x00000000, 0x00000000}};
  while (s21_is_greater_or_equal(value_1, value_2)) {
    s21_decimal value_to_sub = value_2;
    while (s21_is_greater_or_equal(value_1, value_to_sub)) {  // b*2^q<=a
      for (int i = 0; i < q; i++) {
        tmp.bits[0] = tmp.bits[0] | 1 << 2;
        s21_mul(value_to_sub, tmp, &value_to_sub);
        tmp.bits[0] = tmp.bits[0] >> 2;
      }
      q++;
    }
    tmp.bits[q / 32] = tmp.bits[q / 32] | (1 << ((q - 1) % 32));
    s21_add(*result, tmp, result);
    tmp.bits[q / 32] >> ((q - 1) % 32);
    q = 0;
    s21_sub(value_1, value_to_sub, &value_1);
  }
  return res;
}