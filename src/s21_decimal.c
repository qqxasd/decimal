#include "s21_decimal.h"

int main() {
  s21_decimal result;
  s21_add((s21_decimal){{0x79353447, 0x00000004, 0x00000000, 0x00010000}},
          (s21_decimal){{0xD927FFFF, 0xE1003B28, 0x00000004, 0x00140000}},
          &result);

  printf("%X %X %X %X\n", result.bits[0], result.bits[1], result.bits[2],
         result.bits[3]);
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
  int diff = exp1 - exp2;
  if (diff < 0) {
    diff = -diff;
  }
  int flag = 0;
  while (diff > 0 && exp1 != exp2) {
    if (exp2 > exp1) {
      if (!increase_exponent(value_1)) {
        if (!flag) {
          diff = exp2 - diff;
          flag = 1;
        }
        decrease_exponent(value_2);
        exp2 = get_exp(*value_2);
      }
      exp1 = get_exp(*value_1);
    } else {
      if (!increase_exponent(value_2)) {
        if (!flag) {
          diff = exp1 - diff;
          flag = 1;
        }
        decrease_exponent(value_1);
        exp1 = get_exp(*value_1);
      }
      exp2 = get_exp(*value_2);
    }
    diff--;
  }
}

int increase_exponent(s21_decimal *value) {
  int res = 1;
  s21_decimal temp;
  s21_decimal ten = (s21_decimal){{10, 0, 0, 0}};
  if (!s21_mul(*value, ten, &temp)) {
    temp.bits[3] += (1 << 16);
    *value = temp;
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
  if (get_exp(*value) > 28) {
    if (get_sign(*value) && last) {  // banking rounding
      s21_sub(*value, one, value);
    } else if ((getBit(pre_last, 0) && (last > 4)) ||
               (!getBit(pre_last, 0) &&
                (last > 5))) {  // odd prelast  with 5...9 last or even prelast
                                // with 6...9 last
      s21_add(*value, one, value);
    }
  }
}

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = 0;
  *result = (s21_decimal){{0, 0, 0, 0}};
  if (is_zero(value_1)) {
    *result = value_2;
  } else if (is_zero(value_2)) {
    *result = value_1;
  } else {
    equal_exponents(&value_1, &value_2);
    if (get_sign(value_1) == get_sign(value_2)) {
      basic_add(value_1, value_2, result, &res);
    } else if (get_sign(value_1) > get_sign(value_2)) {
      value_1.bits[3] = toggleBit(value_1.bits[3], 31);
      if (s21_is_greater_or_equal(value_2, value_1)) {
        basic_sub(value_2, value_1, result, &res);
      } else {
        basic_sub(value_1, value_2, result, &res);
        (*result).bits[3] = toggleBit((*result).bits[3], 31);
      }
    } else {
      value_2.bits[3] = toggleBit(value_2.bits[3], 31);
      if ((s21_is_greater_or_equal(value_1, value_2) && !get_sign(value_1)) ||
          (s21_is_less_or_equal(value_1, value_2) && get_sign(value_1))) {
        basic_sub(value_1, value_2, result, &res);
      } else {
        basic_sub(value_2, value_1, result, &res);
        (*result).bits[3] = toggleBit((*result).bits[3], 31);
      }
    }
  }
  return res;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = 0;
  *result = (s21_decimal){{0, 0, 0, 0}};
  equal_exponents(&value_1, &value_2);
  if (get_sign(value_1) == get_sign(value_2)) {
    if ((s21_is_greater_or_equal(value_1, value_2))) {
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
  // if (carry) {
  //   basic_add(
  //       (*result),
  //       (s21_decimal){{0x99999999, 0x99999999, 0x99995c40, result->bits[3]}},
  //       result, 0);
  // }
  if (get_exp(*result) > 28) {
    *res = 1;
  }
}

void basic_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result,
               int *res) {
  // s21_decimal carry = {{0, 0, 0, 0}};
  for (int i = 0; i < 96; i++) {
    int bit1 = getBit(value_1.bits[i / 32], i % 32);
    int bit2 = getBit(value_2.bits[i / 32], i % 32);
    int sum;
    int current_bit = i;
    int flag = 0;
    while ((sum = (bit1 - bit2)) < 0) {
      if ((bit1 = getBit(value_1.bits[current_bit / 32], current_bit % 32))) {
        flag = 1;
        while (getBit(value_1.bits[current_bit / 32], current_bit % 32)) {
          value_1.bits[current_bit / 32] =
              clearBit(value_1.bits[current_bit / 32], current_bit % 32);
          while (current_bit >= i) {
            current_bit--;
            value_1.bits[current_bit / 32] =
                setBit(value_1.bits[current_bit / 32], current_bit % 32);
          }

          // carry.bits[(current_bit - 1) / 32] = setBit(
          //     carry.bits[(current_bit - 1) / 32], (current_bit - 1) % 32);
          // current_bit--;
          // carry.bits[(current_bit - 1) / 32] = setBit(
          //     carry.bits[(current_bit - 1) / 32], (current_bit - 1) % 32);
        }
      } else {
        current_bit++;
      }
    }
    if (flag || sum) {
      (*result).bits[i / 32] = setBit((*result).bits[i / 32], i % 32);
      // value_1.bits[current_bit / 32] =
      //  clearBit(value_1.bits[current_bit / 32], current_bit % 32);
    }
    // carry.bits[i / 32] = clearBit(carry.bits[i / 32], (current_bit - 1) %
    // 32);
  }
  (*result).bits[3] = get_sign(value_1) << 31 | (get_exp(value_1)) << 16;
  if (get_exp(*result) < 0) {
    *res = 1;
  }
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = 0;
  int cur_exp = (get_exp(value_1) + get_exp(value_2));
  int cur_sgn = (getBit(get_sign(value_1) + get_sign(value_2), 0));
  *result = (s21_decimal){{0, 0, 0, value_2.bits[3]}};
  if (!(is_zero(value_1) && is_zero(value_2))) {
    if (s21_is_equal(value_2, (s21_decimal){{1, 0, 0, 0}})) {
      *result = value_1;
    } else {
      value_1.bits[3] = clearBit(value_1.bits[3], 31);
      value_2.bits[3] = clearBit(value_2.bits[3], 31);
      for (int i = 0; i < 96 && !res; i++) {
        int bit1 = getBit(value_1.bits[i / 32], i % 32);
        if (bit1) {
          s21_decimal value_to_add = value_2;
          for (int j = 0; j < i; j++) {
            res = s21_add(value_to_add, value_to_add, &value_to_add);
          }
          res = s21_add(*result, value_to_add, result);
        }
      }
      (*result).bits[3] = (*result).bits[3] | (cur_exp << 16 | cur_sgn << 31);
    }
  }
  return res;
}

// void shift_decimal(s21_decimal *value, int shift) {
//   s21_decimal extra_space = (s21_decimal){{0, 0, 0, 0}};
//   s21_decimal res = (s21_decimal){{0, 0, 0, 0}};
//   s21_decimal ten = (s21_decimal){{10, 0, 0, 0}};
//   for (int i = 95; i >= 0; i--) {
//     if (getBit((*value).bits[i / 32], i % 32)) {
//       if (i + shift > 95) {
//         extra_space.bits[(i + shift) / 32] =
//             setBit(extra_space.bits[(i + shift) / 32], i % 32);
//         (*value).bits[i / 32] = clearBit((*value).bits[i / 32], i % 32);
//       } else {
//         (*value).bits[(i + shift) / 32] =
//             setBit((*value).bits[(i + shift) / 32], (i + shift) % 32);
//         (*value).bits[i / 32] = clearBit((*value).bits[i / 32], i % 32);
//       }
//     }
//   }
//   res.bits[3] = (*value).bits[3];
//   normalize_long_decimal(*value, extra_space, &res);
// }

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = 0;
  *result = (s21_decimal){{0, 0, 0, 0}};
  int cur_sgn = getBit(get_sign(value_1) + get_sign(value_2), 0);
  int cur_exp = get_exp(value_1) - get_exp(value_2);
  int q = 0;
  value_1.bits[3] = 0;
  value_2.bits[3] = 0;
  s21_decimal tmp = (s21_decimal){{0, 0, 0, 0}};
  while (s21_is_greater_or_equal(value_1, value_2)) {
    s21_decimal value_to_sub = (s21_decimal){{0, 0, 0, 0}};
    while (s21_is_greater_or_equal(value_1, value_to_sub) &&
           q < 95) {  // b*2^q<=a
      value_to_sub = value_2;
      tmp = value_to_sub;
      for (int i = 0; i < q && s21_is_greater_or_equal(value_1, value_to_sub);
           i++) {
        tmp = value_to_sub;
        s21_add(value_to_sub, value_to_sub, &value_to_sub);
      }
      q++;
    }
    s21_add(*result, tmp, result);
    q = 0;
    s21_sub(value_1, tmp, &value_1);
    tmp = (s21_decimal){{0, 0, 0, 0}};
  }
  result->bits[3] |= (cur_sgn << 31 | cur_exp << 16);
  return res;
}

// int normalize_long_decimal(s21_decimal little, s21_decimal big,
//                            s21_decimal *result) {
//   int res = 0;
//   s21_decimal ten = (s21_decimal){{10, 0, 0, 0}};
//   *result = little;
//   while (!is_zero(big)) {
//     div_long(&little, &big);
//   }
//   (*result) = little;
//   (*result).bits[3] |= 1 << 16;
//   return res;
// }

// void div_long(s21_decimal *little, s21_decimal *big) {  // divides only by 10
//   int res = 0;
//   *result =
//       (s21_decimal){{0, 0, 0,
//                      (getBit(get_sign(value_1) + get_sign(value_2), 0)) << 31
//                      |
//                          (get_exp(value_1) - get_exp(value_2)) << 16}};
//   int q = 0;
//   s21_decimal tmp = (s21_decimal){{0, 0, 0, 0}};
//   while (s21_is_greater_or_equal(value_1, value_2)) {
//     s21_decimal value_to_sub = value_2;
//     while (s21_is_greater_or_equal(value_1, value_to_sub)) {  // b*2^q<=a
//       for (int i = 0; i < q; i++) {
//         basic_sub(value_2, value_1, result, &res);
//         tmp.bits[0] = tmp.bits[0] | 1 << 2;
//         s21_mul(value_to_sub, tmp, &value_to_sub);
//         tmp.bits[0] = tmp.bits[0] >> 2;
//       }
//       q++;
//     }
//     tmp.bits[q / 32] = tmp.bits[q / 32] | (1 << ((q - 1) % 32));
//     s21_add(*result, tmp, result);
//     tmp.bits[q / 32] >> ((q - 1) % 32);
//     q = 0;
//     s21_sub(value_1, value_to_sub, &value_1);
//   }
//   return res;
// }

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