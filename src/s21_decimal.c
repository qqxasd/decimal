#include "s21_decimal.h"

// int main() {
//   s21_decimal result;
//   s21_add((s21_decimal){{0x14490831, 0x00048E23, 0x00000000, 0x00070000}},
//           (s21_decimal){{0x164214B7, 0x00000028, 0x00000000, 0x00040000}},
//           &result);
//   printf("%d %d %d %d\n", result.bits[0], result.bits[1], result.bits[2],
//          get_exp(result));
// }

int setBit(unsigned int num, int pos) { return (num | (1 << pos)); }

int getBit(unsigned int num, int pos) { return ((num & (1 << pos)) >> pos); }

int clearBit(unsigned int num, int pos) { return (num & (~(1 << pos))); }

int toggleBit(unsigned int num, int pos) { return (num ^ (1 << pos)); }

int get_exp(s21_decimal value) { return (value.bits[3] << 10) >> 26; }
int get_sign(s21_decimal value) { return (value.bits[3] >> 31) & 1; }

void equal_exponents(s21_decimal *value_1, s21_decimal *value_2) {
  int exp1 = get_exp(*value_1);
  int exp2 = get_exp(*value_2);
  int diff = exp1 - exp2;
  if (diff < 0) {
    diff = -diff;
  }
  while (diff > 0) {
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
  s21_decimal ten = (s21_decimal){{10, 0, 0, 0}};
  if (!s21_mul(*value, ten, &temp)) {
    *value = temp;
    (*value).bits[3] = (*value).bits[3] | 1 << 16;
  } else {
    res = 0;
  }
  if (get_exp((*value)) > 28) {
    res = 0;
  }
  return res;
}

void decrease_exponent(s21_decimal *value) {
  int last = (*value).bits[0] % 10;
  int pre_last = (*value).bits[0] % 10 % 10;
  s21_decimal ten = (s21_decimal){{10, 0, 0, 0}};
  s21_decimal one = (s21_decimal){{1, 0, 0, 0}};
  s21_div(*value, ten, value);
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
  *result = (s21_decimal){{0, 0, 0, 0}};
  equal_exponents(&value_1, &value_2);
  if (get_sign(value_1) == get_sign(value_2)) {
    basic_add(value_1, value_2, result, &res);
  } else if (get_sign(value_1) > get_sign(value_2)) {
    if ((s21_is_greater_or_equal(value_2, value_1) && !get_sign(value_2)) ||
        (s21_is_less_or_equal(value_2, value_1) && get_sign(value_2))) {
      basic_sub(value_2, value_1, result, &res);
    } else {
      value_1.bits[3] = toggleBit(value_1.bits[3], 31);
      value_2.bits[3] = toggleBit(value_2.bits[3], 31);
      basic_sub(value_1, value_2, result, &res);
      (*result).bits[3] = toggleBit((*result).bits[3], 31);
    }
  } else {
    if ((s21_is_greater_or_equal(value_1, value_2) && !get_sign(value_1)) ||
        (s21_is_less_or_equal(value_1, value_2) && get_sign(value_1))) {
      basic_sub(value_1, value_2, result, &res);
    } else {
      value_1.bits[3] = toggleBit(value_1.bits[3], 31);
      value_2.bits[3] = toggleBit(value_2.bits[3], 31);
      basic_sub(value_2, value_1, result, &res);
      (*result).bits[3] = toggleBit((*result).bits[3], 31);
    }
  }
  return res;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = 0;
  *result = (s21_decimal){{0, 0, 0, 0}};
  equal_exponents(&value_1, &value_2);
  if (get_sign(value_1) == get_sign(value_2)) {
    if ((s21_is_greater_or_equal(value_1, value_2) && !get_sign(value_1)) ||
        (s21_is_less_or_equal(value_1, value_2) && get_sign(value_1))) {
      basic_sub(value_1, value_2, result, &res);
    } else {
      value_1.bits[3] = toggleBit(value_1.bits[3], 31);
      value_2.bits[3] = toggleBit(value_2.bits[3], 31);
      basic_sub(value_2, value_1, result, &res);
      (*result).bits[3] = toggleBit((*result).bits[3], 31);
    }
  } else {
    basic_add(value_1, value_2, result, &res);
  }
  return res;
}

void basic_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result,
               int *res) {
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
                                                    << 16;
}

void basic_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result,
               int *res) {
  s21_decimal carry = {{0, 0, 0, 0}};
  for (int i = 0; i < 96; i++) {
    int bit1 = getBit(value_1.bits[i / 32], i % 32);
    int bit2 = getBit(value_2.bits[i / 32], i % 32);
    int sum;
    int current_bit = i;
    int flag = 0;
    while ((sum = bit1 - bit2 + getBit(carry.bits[i / 32], i % 32)) < 0) {
      if (getBit(value_1.bits[current_bit / 32], current_bit % 32) &&
          current_bit > 0) {
        flag = 1;
        while (getBit(value_1.bits[current_bit / 32], current_bit % 32) &&
               current_bit > 0) {
          value_1.bits[current_bit / 32] =
              clearBit(value_1.bits[current_bit / 32], current_bit % 32);
          value_1.bits[current_bit / 32] = setBit(
              value_1.bits[(current_bit - 1) / 32], (current_bit - 1) % 32);
          carry.bits[i / 32] =
              setBit(carry.bits[i / 32], (current_bit - 1) % 32);
        }
      } else {
        current_bit++;
      }
    }
    if (flag) {
      (*result).bits[i / 32] = setBit((*result).bits[i / 32], i % 32);
      value_1.bits[current_bit / 32] =
          clearBit(value_1.bits[current_bit / 32], current_bit % 32);
    }
    carry.bits[i / 32] = clearBit(carry.bits[i / 32], (current_bit - 1) % 32);
  }
  (*result).bits[3] = get_sign(value_1) << 31 | (get_exp(value_1)) << 16;
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = 0;
  int cur_exp = (get_exp(value_1) + get_exp(value_2));
  int cur_sgn = (getBit(get_sign(value_1) + get_sign(value_2), 0));
  *result = (s21_decimal){{0, 0, 0, 0}};
  value_1.bits[3] = clearBit(value_1.bits[3], 31);
  value_2.bits[3] = clearBit(value_2.bits[3], 31);
  for (int i = 0; i < 96; i++) {
    int bit1 = getBit(value_1.bits[i / 32], i % 32);
    if (bit1) {
      s21_decimal value_to_add = value_2;
      shift_decimal(&value_to_add, i);
      s21_add(*result, value_to_add, result);
    }
  }
  (*result).bits[3] = (*result).bits[3] | (cur_exp << 16 | cur_sgn << 31);
  return res;
}

void shift_decimal(s21_decimal *value, int shift) {
  s21_decimal extra_space = (s21_decimal){{0, 0, 0, 0}};
  s21_decimal res = (s21_decimal){{0, 0, 0, 0}};
  s21_decimal ten = (s21_decimal){{10, 0, 0, 0}};
  for (int i = 95; i >= 0; i--) {
    if (getBit((*value).bits[i / 32], i % 32)) {
      if (i + shift > 95) {
        extra_space.bits[(i + shift) / 32] =
            setBit(extra_space.bits[(i + shift) / 32], i % 32);
        (*value).bits[i / 32] = clearBit((*value).bits[i / 32], i % 32);
      } else {
        (*value).bits[(i + shift) / 32] =
            setBit((*value).bits[(i + shift) / 32], (i + shift) % 32);
        (*value).bits[i / 32] = clearBit((*value).bits[i / 32], i % 32);
      }
    }
  }
  res.bits[3] = (*value).bits[3];
  normalize_long_decimal(*value, extra_space, &res);
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = 0;
  *result =
      (s21_decimal){{0, 0, 0,
                     (getBit(get_sign(value_1) + get_sign(value_2), 0)) << 31 |
                         (get_exp(value_1) - get_exp(value_2)) << 16}};
  int q = 0;
  s21_decimal tmp = (s21_decimal){{0, 0, 0, 0}};
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

int normalize_long_decimal(s21_decimal little, s21_decimal big,
                           s21_decimal *result) {
  int res = 0;
  s21_decimal ten = (s21_decimal){{10, 0, 0, 0}};
  *result = little;
  while (!is_zero(big)) {
    div_long(&little, &big);
  }
  (*result) = little;
  (*result).bits[3] |= 1 << 16;
  return res;
}

void div_long(s21_decimal *little, s21_decimal *big) {  // divides only by 10
  divisions div = {{{0, 0, 0, 0},
                    {0, 0, 0, 0},
                    {0, 0, 0, 0},
                    {0, 0, 0, 0},
                    {0, 0, 0, 0},
                    {0, 0, 0, 0},
                    {0, 0, 0, 0},
                    {0, 0, 0, 0},
                    {0, 0, 0, 0},
                    {0, 0, 0, 0}}};
  s21_decimal one = (s21_decimal){{1, 0, 0, 0}};
  define_divisions(*little, *big, &div);
  basic_sub(*big, div.divided_by_2_big, big, 0);
  if (s21_is_less(*little, div.divided_by_2_little)) {
    basic_sub(*big, one, big, 0);
    basic_add(*little, (s21_decimal){{1 << 31, 0, 0, little->bits[3]}}, little,
              0);
  }  // x/10 = x - x/2 - x/4 - x/8 - (x/2^7)*4 - (x/2^7)*2
  basic_sub(*little, div.divided_by_2_little, little, 0);
  basic_sub(*big, div.divided_by_4_big, big, 0);
  if (s21_is_less(*little, div.divided_by_4_little)) {
    basic_sub(*big, one, big, 0);
    basic_add(*little, (s21_decimal){{1 << 31, 0, 0, little->bits[3]}}, little,
              0);
  }
  basic_sub(*little, div.divided_by_4_little, little, 0);
  basic_sub(*big, div.divided_by_8_big, big, 0);
  if (s21_is_less(*little, div.divided_by_8_little)) {
    basic_sub(*big, one, big, 0);
    basic_add(*little, (s21_decimal){{1 << 31, 0, 0, little->bits[3]}}, little,
              0);
  }
  basic_sub(*little, div.divided_by_8_little, little, 0);
  basic_sub(*big, div.divided_by_128_big_mult_by_4, big, 0);
  if (s21_is_less(*little, div.divided_by_128_little_mult_by_4)) {
    basic_sub(*big, one, big, 0);
    basic_add(*little, (s21_decimal){{1 << 31, 0, 0, little->bits[3]}}, little,
              0);
  }
  basic_sub(*little, div.divided_by_128_little_mult_by_4, little, 0);
  basic_sub(*big, div.divided_by_128_big_mult_by_2, big, 0);
  if (s21_is_less(*little, div.divided_by_128_little_mult_by_2)) {
    basic_sub(*big, one, big, 0);
    basic_add(*little, (s21_decimal){{1 << 31, 0, 0, little->bits[3]}}, little,
              0);
  }
  basic_sub(*little, div.divided_by_128_little_mult_by_2, little, 0);
}

void define_divisions(s21_decimal little, s21_decimal big, divisions *div) {
  div->divided_by_2_little =
      (s21_decimal){{little.bits[0] >> 1 | (little.bits[1] & 1) << 31,
                     little.bits[1] >> 1 | (little.bits[2] & 1) << 31,
                     little.bits[2] >> 1 | (big.bits[1] & 1)}};
  div->divided_by_2_big = (s21_decimal){
      {big.bits[0] >> 1 | (big.bits[1] & 1) << 31,
       big.bits[1] >> 1 | (big.bits[2] & 1) << 31, big.bits[2] >> 1}};
  div->divided_by_8_little =
      (s21_decimal){{little.bits[0] >> 3 | (little.bits[1] & 7) << 29,
                     little.bits[1] >> 3 | (little.bits[2] & 7) << 29,
                     little.bits[2] >> 3 | (big.bits[1] & 7) << 29}};
  div->divided_by_8_big = (s21_decimal){
      {big.bits[0] >> 3 | (big.bits[1] & 7) << 29,
       big.bits[1] >> 3 | (big.bits[2] & 7) << 29, big.bits[2] >> 3}};
  div->divided_by_4_little =
      (s21_decimal){{little.bits[0] >> 2 | (little.bits[1] & 3) << 30,
                     little.bits[1] >> 2 | (little.bits[2] & 3) << 30,
                     little.bits[2] >> 2 | (big.bits[1] & 3) << 30}};
  div->divided_by_4_big = (s21_decimal){
      {big.bits[0] >> 2 | (big.bits[1] & 3) << 30,
       big.bits[1] >> 2 | (big.bits[2] & 3) << 30, big.bits[2] >> 2}};
  div->divided_by_128_little_mult_by_4 =
      (s21_decimal){{little.bits[0] >> 7 << 2 | (little.bits[1] & 127) << 25,
                     little.bits[1] >> 7 << 2 | (little.bits[2] & 127) << 25,
                     little.bits[2] >> 7 << 2 | (big.bits[1] & 127) << 25}};
  div->divided_by_128_big_mult_by_4 =
      (s21_decimal){{big.bits[0] >> 7 << 2 | (big.bits[1] & 127) << 25,
                     big.bits[1] >> 7 << 2 | (big.bits[2] & 127) << 25,
                     big.bits[2] >> 7 << 2}};
  div->divided_by_128_little_mult_by_2 =
      (s21_decimal){{little.bits[0] >> 7 << 1 | (little.bits[1] & 127) << 25,
                     little.bits[1] >> 7 << 1 | (little.bits[2] & 127) << 25,
                     little.bits[2] >> 7 << 1 | (big.bits[1] & 127) << 25}};
  div->divided_by_128_big_mult_by_2 =
      (s21_decimal){{big.bits[0] >> 7 << 1 | (big.bits[1] & 127) << 25,
                     big.bits[1] >> 7 << 1 | (big.bits[2] & 127) << 25,
                     big.bits[2] >> 7 << 1}};
}