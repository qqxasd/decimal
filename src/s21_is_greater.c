#include "s21_decimal.h"

void printBits(size_t const size, void const *const ptr) {
  unsigned char *b = (unsigned char *)ptr;
  unsigned char byte;
  int i, j;

  for (i = size - 1; i >= 0; i--) {
    for (j = 7; j >= 0; j--) {
      byte = (b[i] >> j) & 1;
      printf("%u", byte);
    }
  }
  printf("\n");
}

void set_bit(unsigned int *destination, unsigned int position,
             unsigned int value) {
  if (value)
    *destination |= (1 << position);
  else
    *destination &= ~(1 << position);
}

int is_zero(s21_decimal value) {
  return (!value.bits[0] && !value.bits[1] && !value.bits[2]) ? 1 : 0;
}

void decimal_to_big_decimal(s21_decimal from, s21_big_decimal *to) {
  for (int i = 0; i < 3; i++) {
    to->bits[i] = from.bits[i];
  }
  to->bits[3] = 0;
  to->bits[4] = 0;
  to->bits[5] = 0;
  to->exp = get_exp(from);
}

void shift_right(s21_decimal *value) {
  int store_bit = 0;
  for (int i = 2; i >= 0; i--) {
    int new_bit = getBit(value->bits[i], 0);
    value->bits[i] = value->bits[i] >> 1;
    set_bit(&(value->bits[i]), 31, store_bit);
    store_bit = new_bit;
  }
}

void shift_big_decimal_left(s21_big_decimal *value) {
  int store_bit = 0;
  int new_bit = 0;
  for (int i = 0; i < 6; i++) {
    new_bit = getBit(value->bits[i], 31);
    value->bits[i] = value->bits[i] << 1;
    set_bit(&(value->bits[i]), 0, store_bit);
    store_bit = new_bit;
  }
}

int add_for_multiply(s21_big_decimal value_1, s21_big_decimal value_2,
                     s21_big_decimal *result) {
  int check = 0;
  int store_bit = 0;
  for (int i = 0; i < 192; i++) {
    int a = getBit(value_2.bits[i / 32], i % 32);
    int b = getBit(value_1.bits[i / 32], i % 32);
    set_bit(&(result->bits[i / 32]), i % 32, a ^ b ^ store_bit);
    store_bit = (a && b) || (b && store_bit) || (a && store_bit);
  }
  if (store_bit) check = 1;
  return check;
}

int multiply_10_big(s21_big_decimal *src) {
  s21_big_decimal result = {{0}, 0};
  s21_decimal value_2 = {{10, 0, 0, 0}};
  int check = 0;
  s21_big_decimal tmp_1 = *src;
  while (!is_zero(value_2) && !check) {
    if (getBit(value_2.bits[0], 0))
      check = add_for_multiply(result, tmp_1, &result);
    shift_big_decimal_left(&tmp_1);
    if (getBit(result.bits[5], 31)) check = 1;
    shift_right(&value_2);
  }
  result.exp = src->exp;
  *src = result;
  return check;
}

int normalize_big(s21_big_decimal *x1, s21_big_decimal *x2) {
  int check = 0;
  s21_big_decimal *x_fix;
  int diff;
  if (x1->exp <= x2->exp) {
    x_fix = x1;
    diff = x2->exp - x1->exp;
  } else {
    x_fix = x2;
    diff = x1->exp - x2->exp;
  }
  while (diff--) {
    check = multiply_10_big(x_fix);
    if (check) break;
    x_fix->exp += 1;
  }
  return check;
}

int s21_is_equal(s21_decimal value_1, s21_decimal value_2) {
  if (value_1.bits[0] == value_2.bits[0] &&
      value_1.bits[1] == value_2.bits[1] &&
      value_1.bits[2] == value_2.bits[2] &&
      get_sign(value_1) == get_sign(value_2) &&
      get_exp(value_1) == get_exp(value_2))
    return 1;
  else
    return 0;
}

int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2) {
  return !s21_is_equal(value_1, value_2);
}

int s21_is_greater(s21_decimal value_1, s21_decimal value_2) {
  s21_big_decimal value_1_big, value_2_big;

  decimal_to_big_decimal(value_1, &value_1_big);
  decimal_to_big_decimal(value_2, &value_2_big);

  normalize_big(&value_1_big, &value_2_big);

  int res = 0;
  if (is_zero(value_1) && is_zero(value_2)) {
    res = 0;
  } else if (get_sign(value_1) < get_sign(value_2)) {
    res = 1;
  } else if (get_sign(value_1) > get_sign(value_2)) {
    res = 0;
  } else if (s21_is_equal(value_1, value_2)) {
    res = 0;
  } else {
    for (int i = 191; i >= 0; i--) {
      if (getBit(value_1_big.bits[i / 32], i % 32) >
          getBit(value_2_big.bits[i / 32], i % 32)) {
        res = 1;
        break;
      } else if (getBit(value_1_big.bits[i / 32], i % 32) <
                 getBit(value_2_big.bits[i / 32], i % 32)) {
        res = 0;
        break;
      }
    }
    if (get_sign(value_1) && get_sign(value_2)) res = res ? 0 : 1;
  }
  return res;
}

int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2) {
  int res = 0;
  if (s21_is_greater(value_1, value_2) || s21_is_greater(value_1, value_2)) {
    res = 1;
  }
  return res;
}

int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2) {
  int res = 0;
  if (!s21_is_greater(value_1, value_2)) {
    res = 1;
  }
  return res;
}