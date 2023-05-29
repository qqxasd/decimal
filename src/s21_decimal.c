#include "s21_decimal.h"

#include <string.h>
int main() {
  s21_decimal result;
  // s21_add((s21_decimal){{0x80000000, 0x80000000, 0x80000000, 0x801C0000}},
  //          (s21_decimal){{0x80000000, 0x80000000, 0x80000000, 0x801C0000}},
  //         &result);

  // remainder_div((s21_decimal){{0, 1, 0, 0}},
  //              (s21_decimal){{10, 0, 0, 0x00000000}}, &result, 1);
  s21_mul((s21_decimal){{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}},
          (s21_decimal){{0x00000005, 0x00000000, 0x00000000, 0x00010000}},
          &result);
  //  banking_rounding(&result);
  printf("%X %X %X %X\n", result.bits[0], result.bits[1], result.bits[2],
         result.bits[3]);
  //}
  //   s21_decimal result;
  //   float aboba = 9999999999;
  //   s21_from_float_to_decimal(aboba, &result);
  //   s21_from_decimal_to_float(result, &aboba);
  //   printf("%f\n", aboba);
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

void banking_rounding(s21_decimal *value) {
  s21_decimal one = (s21_decimal){{1, 0, 0, value->bits[3]}};
  int last = (*value).bits[0] % 10;
  int pre_last = (*value).bits[0] % 10 % 10;
  if (get_sign(*value) && last) {  // banking rounding
    s21_sub(*value, one, value);
  } else if ((getBit(pre_last, 0) && (last > 4)) ||
             (!getBit(pre_last, 0) &&
              (last > 5))) {  // odd prelast  with 5...9 last or even
                              // prelast with 6...9 last
    s21_add(*value, one, value);
  }
}

void decrease_exponent(s21_decimal *value) {
  s21_decimal ten = (s21_decimal){{10, 0, 0, 0}};
  s21_div(*value, ten, value);
  value->bits[3] -= (1 << 16);
  if (get_exp(*value) < 0) {
    banking_rounding(value);
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
  if (carry) {
    result->bits[0] =
        (result->bits[0] >> 1) | (getBit(result->bits[1], 0) << 31);
    result->bits[1] =
        (result->bits[1] >> 1) | (getBit(result->bits[2], 0) << 31);
    result->bits[2] = (result->bits[2] >> 1) | (carry << 31);
    s21_div(*result, (s21_decimal){{5, 0, 0, 0x00000000}}, result);
    // s21_mul(*result, (s21_decimal){{2, 0, 0, 0x00000000}}, result);
    // banking_rounding(result);
    // s21_mul(*result, (s21_decimal){{0xFFFFFFFF, 0, 0, 0}}, result);
    //  result->bits[2] |= (1 << 31);
    //  s21_div(*result, (s21_decimal){{10, 0, 0, 0}}, result);
  }
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
          //     carry.bits[(current_bit - 1) / 32], (current_bit - 1)
          //     % 32);
          // current_bit--;
          // carry.bits[(current_bit - 1) / 32] = setBit(
          //     carry.bits[(current_bit - 1) / 32], (current_bit - 1)
          //     % 32);
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
    // carry.bits[i / 32] = clearBit(carry.bits[i / 32], (current_bit
    // - 1) % 32);
  }
  (*result).bits[3] = get_sign(value_1) << 31 | (get_exp(value_1)) << 16;
  if (get_exp(*result) < 0) {
    *res = 1;
  }
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = 0;
  int bits = overflow_check(value_1, value_2);
  int cur_exp = (get_exp(value_1) + get_exp(value_2));
  int cur_sgn = (getBit(get_sign(value_1) + get_sign(value_2), 0));
  if (!(is_zero(value_1) && is_zero(value_2))) {
    if (bits < 96) {
      basic_mul(value_1, value_2, result);
    } else {
      approximate_mul(value_1, value_2, result, &res);
    }
  } else {
    *result = (s21_decimal){{0, 0, 0, 0}};
  }
  (*result).bits[3] = (cur_exp << 16 | cur_sgn << 31);
  // while (get_exp(*result) > 28) {
  //   decrease_exponent(result);
  // }
  if (is_zero(*result) && get_exp(*result) > 0) {
    res = 2;
  }
  return res;
}

int approximate_mul(s21_decimal value_1, s21_decimal value_2,
                    s21_decimal *result, int *res) {
  int exp = 0;
  value_1.bits[3] = 0;
  value_2.bits[3] = 0;
  s21_decimal value_1_cpy = value_1;
  s21_decimal value_2_cpy = value_2;
  while (overflow_check(value_1_cpy, value_2_cpy) > 96) {
    exp++;
    remainder_div(value_1_cpy, (s21_decimal){{10, 0, 0, 0}}, &value_1_cpy, 0);
    remainder_div(value_2_cpy, (s21_decimal){{10, 0, 0, 0}}, &value_2_cpy, 0);
  }
  s21_decimal ten = (s21_decimal){{10, 0, 0, 0}};
  for (int i = 0; i < exp; i++) {
    basic_mul(ten, (s21_decimal){{10, 0, 0, 0}}, &ten);
  }
  s21_decimal tmp;
  s21_decimal m;
  remainder_div(value_1, ten, &m, 0);
  // m.bits[3] |= (get_exp(value_1) << 16);
  s21_decimal n;
  remainder_div(value_2, ten, &n, 0);
  // n.bits[3] |= (get_exp(value_2) << 16);
  s21_decimal p;
  basic_mul(m, ten, &tmp);
  basic_sub(value_1, tmp, &p, 0);
  s21_decimal q;
  basic_mul(n, ten, &tmp);
  basic_sub(value_2, tmp, &q, 0);
  s21_mul(m, n, &tmp);
  s21_mul(tmp, ten, result);
  s21_decimal mq;
  s21_mul(m, q, &mq);
  basic_add(*result, mq, result, 0);
  s21_decimal pn;
  s21_mul(p, n, &pn);
  basic_add(*result, pn, result, 0);
  s21_decimal pq;
  int x1 = approximate_mul(p, q, &pq, res);
  ten = (s21_decimal){{10, 0, 0, 0}};
  for (int i = 0; i < exp - x1; i++) {
    basic_mul(ten, (s21_decimal){{10, 0, 0, 0}}, &ten);
  }
  remainder_div(pq, (s21_decimal){{10, 0, 0, 0}}, &pq, 0);
  basic_add(*result, pq, result, 0);
  // result->bits[3] = (getBit(get_sign(value_1) + get_sign(value_2), 0) << 31
  // |
  //                    (get_exp(value_1) + get_exp(value_2) - exp) << 16);
  return exp;
}

int overflow_check(s21_decimal value_1, s21_decimal value_2) {
  int res = 0;
  int flag = 0;
  for (int i = 95; i >= 0 & !flag; i--) {
    if (getBit(value_1.bits[i / 32], i % 32)) {
      flag = 1;
      res += i;
    }
  }
  flag = 0;
  for (int i = 95; i >= 0 & !flag; i--) {
    if (getBit(value_2.bits[i / 32], i % 32)) {
      flag = 1;
      res += i;
    }
  }
  return res;
}

void basic_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  *result = (s21_decimal){{0, 0, 0, 0}};
  if (s21_is_equal(value_2, (s21_decimal){{1, 0, 0, 0}})) {
    *result = value_1;
  } else {
    value_1.bits[3] = clearBit(value_1.bits[3], 31);
    value_2.bits[3] = clearBit(value_2.bits[3], 31);
    for (int i = 0; i < 96; i++) {
      int bit1 = getBit(value_1.bits[i / 32], i % 32);
      if (bit1) {
        s21_decimal value_to_add = value_2;
        for (int j = 0; j < i; j++) {
          s21_add(value_to_add, value_to_add, &value_to_add);
        }
        s21_add(*result, value_to_add, result);
      }
    }
  }
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
//         (*value).bits[i / 32] = clearBit((*value).bits[i / 32], i %
//         32);
//       } else {
//         (*value).bits[(i + shift) / 32] =
//             setBit((*value).bits[(i + shift) / 32], (i + shift) %
//             32);
//         (*value).bits[i / 32] = clearBit((*value).bits[i / 32], i %
//         32);
//       }
//     }
//   }
//   res.bits[3] = (*value).bits[3];
//   normalize_long_decimal(*value, extra_space, &res);
// }

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = 0;
  if (is_zero(value_2)) {
    res = 3;
  } else {
    *result = (s21_decimal){{0, 0, 0, 0}};
    int cur_sgn = getBit(get_sign(value_1) + get_sign(value_2), 0);
    int cur_exp;
    //  while ((cur_exp = get_exp(value_1) - get_exp(value_2)) < 0) {
    //   increase_exponent(&value_1);
    //  }
    cur_exp = remainder_div(value_1, value_2, result, 1);
    banking_rounding(result);
    result->bits[3] |= (cur_sgn << 31 | cur_exp << 16);
  }
  return res;
}

int remainder_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result,
                  int precise) {
  int q = 1;
  int cur_exp = get_exp(value_1) - get_exp(value_2);
  int max_shift = 0;
  for (int i = 95; i >= 0 && !max_shift; i--) {
    if (getBit(value_2.bits[i / 32], i % 32)) {
      max_shift = 96 - i;
    }
  }
  *result = (s21_decimal){{0, 0, 0, 0}};
  value_1.bits[3] = 0;
  value_2.bits[3] = 0;
  s21_decimal tmp = (s21_decimal){{0, 0, 0, 0}};
  while (s21_is_greater_or_equal(value_1, value_2)) {
    s21_decimal value_to_add = value_2;
    while (s21_is_greater_or_equal(value_1, value_to_add) && q < max_shift &&
           !getBit(result->bits[2], 31)) {  // b*2^q<=a
      value_to_add = value_2;
      tmp = value_to_add;
      for (int i = 0; i < q && !(getBit(tmp.bits[2], 30)); i++) {
        tmp = value_to_add;
        s21_add(value_to_add, value_to_add, &value_to_add);
      }
      q++;
    }
    q = q - 2;
    value_to_add = (s21_decimal){{0, 0, 0, 0}};
    value_to_add.bits[q / 32] = (1 << (q % 32));
    s21_add(*result, value_to_add, result);
    q = 0;
    s21_sub(value_1, tmp, &value_1);
    tmp = (s21_decimal){{0, 0, 0, 0}};
    if (precise && !(is_zero(value_1)) && s21_is_less(value_1, value_2) &&
        (cur_exp < 28) && !(result->bits[2] >> 28)) {
      basic_mul(value_1, (s21_decimal){{10, 0, 0, 0}}, &value_1);
      basic_mul(*result, (s21_decimal){{10, 0, 0, 0}}, result);
      cur_exp++;
    }
  }
  result->bits[3] = 0;
  return cur_exp;
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

// void div_long(s21_decimal *little, s21_decimal *big) {  // divides
// only by 10
//   int res = 0;
//   *result =
//       (s21_decimal){{0, 0, 0,
//                      (getBit(get_sign(value_1) + get_sign(value_2),
//                      0)) << 31
//                      |
//                          (get_exp(value_1) - get_exp(value_2)) <<
//                          16}};
//   int q = 0;
//   s21_decimal tmp = (s21_decimal){{0, 0, 0, 0}};
//   while (s21_is_greater_or_equal(value_1, value_2)) {
//     s21_decimal value_to_sub = value_2;
//     while (s21_is_greater_or_equal(value_1, value_to_sub)) {  //
//     b*2^q<=a
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
  if (s21_is_greater(value_1, value_2) || s21_is_equal(value_1, value_2)) {
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

int s21_is_less(s21_decimal value_1, s21_decimal value_2) {
  int res = 0;
  if (!s21_is_greater(value_1, value_2) && !s21_is_equal(value_1, value_2)) {
    res = 1;
  }
  return res;
}

void nulify_dec(s21_decimal *dst) {
  dst->bits[0] = 0;
  dst->bits[1] = 0;
  dst->bits[2] = 0;
  dst->bits[3] = 0;
}

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  nulify_dec(dst);
  if (src & 1 << 31) {
    dst->bits[3] |= 1 << 31;
    dst->bits[0] |= ~src + 1;
  } else {
    dst->bits[0] = src;
  }
  return 0;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int ret = 0;
  while (get_exp(src)) {
    decrease_exponent(&src);
  }
  if (src.bits[0] & 1 << 31 || src.bits[1] || src.bits[1] & 1 << 31 ||
      src.bits[2] || src.bits[2] & 1 << 31) {
    ret = 1;
  } else {
    *dst = src.bits[0];
    if (get_sign(src)) {
      *dst *= -1;
    }
  }
  return ret;
}

int count_digits(long n) {
  int count = 0;
  if (n < 0) {
    n *= -1;
  }
  while (n >= 1) {
    n /= 10;
    count++;
  }
  return count;
}

int count_pre_null(float n) {
  int i = 0;
  if ((int)n == 0) {
    for (i = 1; i < 9 && (int)(n * pow(10, i)) == 0; i++)
      ;
  }
  return i - 1;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  if (fabs(src) < 1e-28 || src > MAX_DEC_VALUE || isinf(src) != 0) {
    return 1;
  }
  nulify_dec(dst);
  if (src < 0) {
    dst->bits[3] |= 1 << 31;
  }
  long tmp = (long)src;
  src -= tmp;
  int exp = 7 - count_digits(tmp);
  if (exp < 0) {
    while (count_digits(tmp) > 7) {
      tmp /= 10;
    }
    exp = 0;
  } else {
    src *= pow(10, exp - count_pre_null(src));
    tmp *= pow(10, exp - count_pre_null(src) - 1);
  }
  tmp += src;
  if (tmp < 0) {
    tmp = ~tmp + 1;
  }
  dst->bits[0] = tmp;
  dst->bits[3] |= (exp << 16);
  return 0;
}

void dec_to_str(s21_decimal src, char *str) {
  int count = 0;
  if (get_sign(src)) {
    str[0] = '-';
    count++;
  }
  for (int i = 2; i >= 0; i--) {
    for (int j = count_digits(src.bits[i]) - 1; j >= 0; j--) {
      str[count++] = ((int)(src.bits[i] / pow(10, j))) % 10 + '0';
    }
  }
  str[count] = '\0';
  char tmp[64];
  strcpy(tmp, &str[count - 1 - get_exp(src)]);
  str[strlen(str) - get_exp(src) - 1] = '.';
  strcpy(&str[strlen(str) - get_exp(src)], tmp);
  str[get_exp(src) + strlen(tmp) + 1] = '\0';
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  *dst = src.bits[2];
  *dst *= pow(10, count_digits(src.bits[2]));
  *dst += src.bits[1];
  *dst *= pow(10, count_digits(src.bits[1]));
  *dst += src.bits[0];
  *dst /= pow(10, get_exp(src));
  return 0;
}
