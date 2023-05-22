#include "s21_decimal.h"

typedef struct {
  int bits[6];
  int exp;
} s21_big_decimal;

int get_bit(unsigned int source, unsigned int position) {
  return (source & (1 << position));
}

void set_bit(unsigned int *destination, unsigned int position, int value) {
  if (value)
    *destination |= (1 << position);
  else
    *destination &= ~(1 << position);
}

int is_zero(s21_decimal value) {
  return (!value.bits[0] && !value.bits[1] && !value.bits[2]) ? 1 : 0;
}

void to_big(s21_decimal from, s21_big_decimal *to) {
  for (int i = 0; i < 3; i++) {
    to->bits[i] = from.bits[i];
  }
  to->bits[3] = 0;
  to->bits[4] = 0;
  to->bits[5] = 0;
  to->exp = s21_get_exp(from);
}
void shift_right(s21_decimal *value) {
  int store_bit = 0;
  for (int i = 2; i >= 0; i--) {
    int new_bit = get_bit(value->bits[i], 0);
    value->bits[i] = value->bits[i] >> 1;
    set_bit(&(value->bits[i]), 31, store_bit);
    store_bit = new_bit;
  }
}

void shift_big_left(s21_big_decimal *value) {
  int store_bit = 0;
  for (int i = 0; i < 6; i++) {
    int new_bit = get_bit(value->bits[i], 31);
    value->bits[i] = value->bits[i] << 1;
    set_bit(&(value->bits[i]), 0, store_bit);
    store_bit = new_bit;
  }
}

int add_for_mul(s21_big_decimal res_value, s21_big_decimal value_1,
                s21_big_decimal *res) {
  int check = 0;
  int add_bit = 0;
  for (int i = 0; i < 192; i++) {
    int a = get_bit(value_1.bits[i / 32], i % 32);
    int b = get_bit(res_value.bits[i / 32], i % 32);
    set_bit(&(res->bits[i / 32]), i % 32, a ^ b ^ add_bit);
    add_bit = (a && b) || (b && add_bit) || (a && add_bit);
  }
  if (add_bit) check = 1;
  return check;
}

int s21_mul10_big(s21_big_decimal *src) {
  s21_big_decimal res = {{0}, 0};
  s21_decimal value_2 = {{10, 0, 0, 0}};
  int check = 0;
  s21_big_decimal tmp_1 = *src;
  while (!is_null(value_2) && !check) {
    if (get_bit(value_2.bits[0], 0)) check = add_for_mul(res, tmp_1, &res);
    left_shift_big(&tmp_1);
    if (get_bit(res.bits[5], 31)) check = 1;
    right_shift(&value_2);
  }
  res.exp = src->exp;
  *src = res;
  return check;
}

int s21_normalize_big(s21_big_decimal *x1, s21_big_decimal *x2) {
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
    check = s21_mul10_big(x_fix);
    if (check) break;
    x_fix->exp += 1;
  }
  return check;
}

int s21_is_greater(s21_decimal a, s21_decimal b) {
  s21_big_decimal a_big, b_big;

  to_big(a, &a_big);
  to_big(b, &b_big);

  s21_normalize_big(&a_big, &b_big);

  int res = 0;
  if (is_zero(a, b)) {
    res = 0;
  } else if (s21_get_sign(a) < s21_get_sign(b)) {
    res = 1;
  } else if (s21_get_sign(a) > s21_get_sign(b)) {
    res = 0;
  } else if (s21_is_equal(a, b)) {
    res = 0;
  } else {
    for (int i = 191; i >= 0; i--) {
      if (get_bit(a_big.bits[i / 32], i % 32) >
          get_bit(b_big.bits[i / 32], i % 32)) {
        res = 1;
        break;
      } else if (get_bit(a_big.bits[i / 32], i % 32) <
                 get_bit(b_big.bits[i / 32], i % 32)) {
        res = 0;
        break;
      }
    }
    if (s21_get_sign(a) && s21_get_sign(b)) res = res ? 0 : 1;
  }
  return res;
}