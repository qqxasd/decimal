#include "s21_decimal.h"

#include <string.h>
// int main()
// {
//   s21_decimal result;
//   // s21_decimal temp = (s21_decimal){{0xD927FFFF, 0xE1003B28, 0x00000004,
//   //                                   0x00140000}};
//   // printf("OLD \n(0x%X, 0x%X, 0x%X, 0x%X),\n", temp.bits[0], temp.bits[1], temp.bits[2],
//   //        temp.bits[3]);
//   // for (int i = 0; i < 10; i++)
//   // {
//   //   decrease_exponent(&temp);
//   //   printf("(0x%X, 0x%X, 0x%X, 0x%X),\n", temp.bits[0], temp.bits[1], temp.bits[2],
//   //          temp.bits[3]);
//   // }

//   s21_add((s21_decimal){{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF,
//                          0x00000000}},
//           (s21_decimal){{0x00000005, 0x00000000, 0x00000000, 0x00010000}},
//           &result);
//   // s21_div((s21_decimal){{94534286, 1642445539, 544598, 0}},
//   //         (s21_decimal){{10, 0, 0, 0}}, &result);
//   printf("(0x%X, 0x%X, 0x%X, 0x%X),\n", result.bits[0], result.bits[1], result.bits[2],
//          result.bits[3]);
// }
// remainder_div((s21_decimal){{0, 1, 0, 0}},
//              (s21_decimal){{10, 0, 0, 0x00000000}}, &result, 1);
// s21_mul((s21_decimal){{0x00000000, 0, 0xB0000000, 0x00100000}},
//         (s21_decimal){{2, 0, 0, 0x00010000}}, &result);
//  banking_rounding(&result);
//   s21_decimal result;
//   float aboba = 9999999999;
//   s21_from_float_to_decimal(aboba, &result);
//   s21_from_decimal_to_float(result, &aboba);
//   printf("%f\n", aboba);

int setBit(unsigned int num, int pos) { return (num | (1 << pos)); }

int getBit(unsigned int num, int pos) { return ((num & (1 << pos)) >> pos); }

int clearBit(unsigned int num, int pos) { return (num & (~(1 << pos))); }

int toggleBit(unsigned int num, int pos) { return (num ^ (1 << pos)); }

int get_exp(s21_decimal value) { return (value.bits[3] << 10) >> 26; }
int get_sign(s21_decimal value) { return (value.bits[3] >> 31) & 1; }

void equal_exponents(s21_decimal *value_1, s21_decimal *value_2)
{
  int exp1 = get_exp(*value_1);
  int exp2 = get_exp(*value_2);
  int decreased = 10;
  int val_num = 2;
  while (exp1 != exp2)
  {
    if (exp2 > exp1)
    {
      if (!increase_exponent(value_1))
      {
        decreased = decrease_exponent(value_2);
        exp2 = get_exp(*value_2);
        val_num = 0;
      }
      exp1 = get_exp(*value_1);
    }
    else
    {
      if (!increase_exponent(value_2))
      {
        decreased = decrease_exponent(value_1);
        exp1 = get_exp(*value_1);
        val_num = 1;
      }
      exp2 = get_exp(*value_2);
    }
  }
  if (decreased < 10)
  {
    if (val_num)
    {
      if (decreased > 4)
      {
        s21_add(*value_1, (s21_decimal){{1, 0, 0, value_1->bits[3]}}, value_1);
      }
    }
    else
    {
      if (decreased > 4)
      {
        s21_add(*value_2, (s21_decimal){{1, 0, 0, value_2->bits[3]}}, value_2);
      }
    }
  }
}

int increase_exponent(s21_decimal *value)
{
  int res = 1;
  s21_decimal temp = *value;
  if (!mul_by_10(&temp))
  {
    *value = temp;
    value->bits[3] += (1 << 16);
  }
  else
  {
    res = 0;
  }
  if (get_exp((*value)) > 28 || is_zero(*value))
  {
    res = 0;
  }
  return res;
}

void last_prelast(s21_decimal value, int *last, int *prelast)
{
  *last = (value.bits[0] + value.bits[1] + value.bits[2]) % 10;
  *prelast = (value.bits[0] + value.bits[1] + value.bits[2]) / 10 % 10;
}

void banking_rounding(s21_decimal *value, int dont_divide)
{
  int last, pre_last;
  last_prelast(*value, &last, &pre_last);
  if (!dont_divide)
  {
    s21_div(*value, (s21_decimal){{10, 0, 0, 0}}, value);
  }
  s21_decimal one = (s21_decimal){{1, 0, 0, value->bits[3]}};
  if (get_sign(*value) && last)
  {
    s21_sub(*value, one, value);
  }
  else if ((getBit(pre_last, 0) && (last > 4)) ||
           (!getBit(pre_last, 0) &&
            (last > 5)))
  { // odd prelast  with 5...9 last or even
    // prelast with 6...9 last
    s21_add(*value, one, value);
  }
}

int decrease_exponent(s21_decimal *value)
{
  int last = div_by_10(*value, value, 0);
  // printf("(0x%X, 0x%X, 0x%X, 0x%X),\n", value->bits[0], value->bits[1], value->bits[2],
  //        value->bits[3]);
  value->bits[3] -= (1 << 16);
  if (get_exp(*value) < 0)
  {
    banking_rounding(value, 0);
  }
  return last;
}

int mul_by_10(s21_decimal *value)
{
  int res = 0;
  s21_decimal value_2 = (s21_decimal){{value->bits[0] << 1, (value->bits[1] << 1) | getBit(value->bits[0], 31), (value->bits[2] << 1) | getBit(value->bits[1], 31), value->bits[3]}};

  s21_decimal value_8 = (s21_decimal){{value->bits[0] << 3, (value->bits[1] << 3) | (value->bits[0] >> 29), (value->bits[2] << 3) | (value->bits[1] >> 29), value->bits[3]}};
  // printf("2 \n(0x%X, 0x%X, 0x%X, 0x%X),\n", value_2.bits[0], value_2.bits[1], value_2.bits[2],
  //        value_2.bits[3]);
  // printf("8 \n(0x%X, 0x%X, 0x%X, 0x%X),\n", value_8.bits[0], value_8.bits[1], value_8.bits[2],
  //        value_8.bits[3]);
  if (!(value->bits[2] >> 28))
  {
    s21_add(value_2, value_8, value);
  }
  else
  {
    res = 1;
  }
  return res;
}

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result)
{
  int res = 0;
  *result = (s21_decimal){{0, 0, 0, 0}};
  if (is_zero(value_1))
  {
    *result = value_2;
  }
  else if (is_zero(value_2))
  {
    *result = value_1;
  }
  else
  {
    equal_exponents(&value_1, &value_2);
    // printf("(0x%X, 0x%X, 0x%X, 0x%X),\n", value_1.bits[0], value_1.bits[1], value_1.bits[2],
    //        value_1.bits[3]);
    if (get_sign(value_1) == get_sign(value_2))
    {
      basic_add(value_1, value_2, result, &res);
    }
    else if (get_sign(value_1) > get_sign(value_2))
    {
      value_1.bits[3] = toggleBit(value_1.bits[3], 31);
      if (s21_is_greater_or_equal(value_2, value_1))
      {
        basic_sub(value_2, value_1, result, &res);
      }
      else
      {
        basic_sub(value_1, value_2, result, &res);
        (*result).bits[3] = toggleBit((*result).bits[3], 31);
      }
    }
    else
    {
      value_2.bits[3] = toggleBit(value_2.bits[3], 31);
      if ((s21_is_greater_or_equal(value_1, value_2) && !get_sign(value_1)) ||
          (s21_is_less_or_equal(value_1, value_2) && get_sign(value_1)))
      {
        basic_sub(value_1, value_2, result, &res);
      }
      else
      {
        basic_sub(value_2, value_1, result, &res);
        (*result).bits[3] = toggleBit((*result).bits[3], 31);
      }
    }
  }
  return res;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result)
{
  int res = 0;
  *result = (s21_decimal){{0, 0, 0, 0}};
  equal_exponents(&value_1, &value_2);
  if (get_sign(value_1) == get_sign(value_2))
  {
    if ((s21_is_greater_or_equal(value_1, value_2)))
    {
      basic_sub(value_1, value_2, result, &res);
    }
    else
    {
      value_1.bits[3] = toggleBit(value_1.bits[3], 31);
      value_2.bits[3] = toggleBit(value_2.bits[3], 31);
      basic_sub(value_2, value_1, result, &res);
      (*result).bits[3] = toggleBit((*result).bits[3], 31);
    }
  }
  else
  {
    basic_add(value_1, value_2, result, &res);
  }
  return res;
}

int basic_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result,
              int *res)
{
  int carry = 0;
  for (int i = 0; i < 96; i++)
  {
    int bit1 = getBit(value_1.bits[i / 32], i % 32);
    int bit2 = getBit(value_2.bits[i / 32], i % 32);
    int sum = bit1 + bit2 + carry;
    carry = getBit(sum, 1);
    if (getBit(sum, 0))
    {
      (*result).bits[i / 32] = setBit((*result).bits[i / 32], i % 32);
    }
  }
  if (carry)
  {
    int last = div_by_10(*result, result, 1);
    if ((getBit(result->bits[0], 0) && (last == 5)) || (last > 5) || (get_sign(*result) && last))
    {
      s21_add(*result, (s21_decimal){{1, 0, 0, result->bits[3]}}, result);
    }
  }
  (*result).bits[3] = get_sign(value_1) << 31 | (get_exp(value_1) - carry)
                                                    << 16;
  if (get_exp(*result) > 28)
  {
    *res = 1;
  }
  return carry;
}

void basic_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result,
               int *res)
{
  *result = (s21_decimal){{0, 0, 0, 0}};
  for (int i = 0; i < 96; i++)
  {
    int bit1 = getBit(value_1.bits[i / 32], i % 32);
    int bit2 = getBit(value_2.bits[i / 32], i % 32);
    int sum;
    int current_bit = i;
    int flag = 0;
    while ((sum = (bit1 - bit2)) < 0)
    {
      if ((bit1 = getBit(value_1.bits[current_bit / 32], current_bit % 32)))
      {
        flag = 1;
        value_1.bits[current_bit / 32] =
            clearBit(value_1.bits[current_bit / 32], current_bit % 32);
        while (current_bit >= i)
        {
          current_bit--;
          value_1.bits[current_bit / 32] =
              setBit(value_1.bits[current_bit / 32], current_bit % 32);
        }
      }
      else
      {
        current_bit++;
      }
    }
    if (flag || sum)
    {
      (*result).bits[i / 32] = setBit((*result).bits[i / 32], i % 32);
    }
  }
  (*result).bits[3] = get_sign(value_1) << 31 | (get_exp(value_1)) << 16;
  if (get_exp(*result) < 0)
  {
    *res = 1;
  }
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result)
{
  int res = 0;
  int bits = overflow_check(value_1, value_2);
  int cur_exp = (get_exp(value_1) + get_exp(value_2));
  int cur_sgn = (getBit(get_sign(value_1) + get_sign(value_2), 0));
  if (!(is_zero(value_1) && is_zero(value_2)))
  {
    if (bits < 96)
    {
      basic_mul(value_1, value_2, result);
    }
    else
    {
      cur_exp += long_mul(value_1, value_2, result);
    }
  }
  else
  {
    *result = (s21_decimal){{0, 0, 0, 0}};
  }
  (*result).bits[3] = (cur_exp << 16 | cur_sgn << 31);
  while (get_exp(*result) > 28)
  {
    decrease_exponent(result);
  }
  if (is_zero(*result) && get_exp(*result) > 0)
  {
    res = 2;
  }
  return res;
}

int long_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result)
{
  int exp = 0;
  *result = (s21_decimal){{0, 0, 0, 0}};
  s21_big_decimal long_result = (s21_big_decimal){{0, 0, 0, 0, 0, 0}, 0};
  big_mul(
      (s21_big_decimal){
          {value_1.bits[0], value_1.bits[1], value_1.bits[2], 0, 0, 0}, 0},
      (s21_big_decimal){
          {value_2.bits[0], value_2.bits[1], value_2.bits[2], 0, 0, 0}, 0},
      &long_result);
  exp = big_decimal_to_decimal(long_result, result);
  return exp;
}

int big_decimal_to_decimal(s21_big_decimal long_result, s21_decimal *result)
{
  s21_big_decimal cpy = long_result;
  int exp = 0;
  while (big_is_greater_or_equal(
      cpy,
      (s21_big_decimal){{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0, 0, 0}, 0}))
  {
    exp++;
    remainder_big_div(cpy, (s21_big_decimal){{10, 0, 0, 0, 0, 0}, 0}, &cpy);
  }
  result->bits[0] = long_result.bits[0];
  result->bits[1] = long_result.bits[1];
  result->bits[2] = long_result.bits[2];
  return exp;
}

void big_add(s21_big_decimal value_1, s21_big_decimal value_2,
             s21_big_decimal *result)
{
  *result = (s21_big_decimal){{0, 0, 0, 0, 0, 0}, 0};
  int carry = 0;
  for (int i = 0; i < 192; i++)
  {
    int bit1 = getBit(value_1.bits[i / 32], i % 32);
    int bit2 = getBit(value_2.bits[i / 32], i % 32);
    int sum = bit1 + bit2 + carry;
    carry = getBit(sum, 1);
    if (getBit(sum, 0))
    {
      (*result).bits[i / 32] = setBit((*result).bits[i / 32], i % 32);
    }
  }
}

void big_sub(s21_big_decimal value_1, s21_big_decimal value_2,
             s21_big_decimal *result)
{
  *result = (s21_big_decimal){{0, 0, 0, 0, 0, 0}, 0};
  for (int i = 0; i < 192; i++)
  {
    int bit1 = getBit(value_1.bits[i / 32], i % 32);
    int bit2 = getBit(value_2.bits[i / 32], i % 32);
    int sum;
    int current_bit = i;
    int flag = 0;
    while ((sum = (bit1 - bit2)) < 0)
    {
      if ((bit1 = getBit(value_1.bits[current_bit / 32], current_bit % 32)))
      {
        flag = 1;
        while (getBit(value_1.bits[current_bit / 32], current_bit % 32))
        {
          value_1.bits[current_bit / 32] =
              clearBit(value_1.bits[current_bit / 32], current_bit % 32);
          while (current_bit >= i)
          {
            current_bit--;
            value_1.bits[current_bit / 32] =
                setBit(value_1.bits[current_bit / 32], current_bit % 32);
          }
        }
      }
      else
      {
        current_bit++;
      }
    }
    if (flag || sum)
    {
      (*result).bits[i / 32] = setBit((*result).bits[i / 32], i % 32);
    }
  }
}

void big_mul(s21_big_decimal value_1, s21_big_decimal value_2,
             s21_big_decimal *result)
{
  *result = (s21_big_decimal){{0, 0, 0, 0, 0, 0}, 0};
  for (int i = 0; i < 192; i++)
  {
    int bit1 = getBit(value_1.bits[i / 32], i % 32);
    if (bit1)
    {
      s21_big_decimal value_to_add = value_2;
      for (int j = 0; j < i; j++)
      {
        big_add(value_to_add, value_to_add, &value_to_add);
      }
      big_add(*result, value_to_add, result);
    }
  }
}

void remainder_big_div(s21_big_decimal value_1, s21_big_decimal value_2,
                       s21_big_decimal *result)
{
  int q = 1;
  int cur_exp = 0;
  int max_shift = 0;
  for (int i = 191; i >= 0 && !max_shift; i--)
  {
    if (getBit(value_2.bits[i / 32], i % 32))
    {
      max_shift = 192 - i;
    }
  }
  s21_big_decimal tmp = (s21_big_decimal){{0, 0, 0, 0, 0, 0}, 0};
  while (big_is_greater_or_equal(value_1, value_2))
  {
    s21_big_decimal value_to_add = value_2;
    while (big_is_greater_or_equal(value_1, value_to_add) &&
           q < max_shift)
    { // b*2^q<=a
      value_to_add = value_2;
      tmp = value_to_add;
      for (int i = 0; i < q && !(getBit(tmp.bits[5], 30)); i++)
      {
        tmp = value_to_add;
        big_add(value_to_add, value_to_add, &value_to_add);
      }
      q++;
    }
    q = q - 2;
    value_to_add = (s21_big_decimal){{0, 0, 0, 0, 0, 0}, 0};
    value_to_add.bits[q / 32] = (1 << (q % 32));
    big_add(*result, value_to_add, result);
    q = 0;
    while (!(big_is_zero(value_1)) && !big_is_greater_or_equal(value_1, value_2) &&
           (cur_exp < 28) && (result->bits[2] < 4294967295 / 10))
    {
      multiply_10_big(&value_1);
      multiply_10_big(result);
      cur_exp++;
    }
  }
}

int big_is_greater_or_equal(s21_big_decimal value_1, s21_big_decimal value_2)
{
  int res = 0;
  s21_decimal value_1_big =
      (s21_decimal){{value_1.bits[3], value_1.bits[4], value_1.bits[5], 0}};
  s21_decimal value_1_small =
      (s21_decimal){{value_1.bits[0], value_1.bits[1], value_1.bits[2], 0}};
  s21_decimal value_2_big =
      (s21_decimal){{value_2.bits[3], value_2.bits[4], value_2.bits[5], 0}};
  s21_decimal value_2_small =
      (s21_decimal){{value_2.bits[0], value_2.bits[1], value_2.bits[2], 0}};
  if (s21_is_greater(value_1_big, value_2_big) ||
      (s21_is_equal(value_1_big, value_2_big) &&
       (s21_is_equal(value_1_small, value_2_small) ||
        s21_is_greater(value_1_small, value_2_small))))
  {
    res = 1;
  }
  return res;
}

int overflow_check(s21_decimal value_1, s21_decimal value_2)
{
  int res = 0;
  int flag = 0;
  for (int i = 95; (i >= 0) & !flag; i--)
  {
    if (getBit(value_1.bits[i / 32], i % 32))
    {
      flag = 1;
      res += i;
    }
  }
  flag = 0;
  for (int i = 95; (i >= 0) & !flag; i--)
  {
    if (getBit(value_2.bits[i / 32], i % 32))
    {
      flag = 1;
      res += i;
    }
  }
  return res;
}

void basic_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result)
{
  *result = (s21_decimal){{0, 0, 0, 0}};
  if (s21_is_equal(value_2, (s21_decimal){{1, 0, 0, 0}}))
  {
    *result = value_1;
  }
  else
  {
    value_1.bits[3] = clearBit(value_1.bits[3], 31);
    value_2.bits[3] = clearBit(value_2.bits[3], 31);
    for (int i = 0; i < 96; i++)
    {
      int bit1 = getBit(value_1.bits[i / 32], i % 32);
      if (bit1)
      {
        s21_decimal value_to_add = value_2;
        for (int j = 0; j < i; j++)
        {
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

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result)
{
  int res = 0;
  if (is_zero(value_2))
  {
    res = 3;
  }
  else
  {
    *result = (s21_decimal){{0, 0, 0, 0}};
    int cur_sgn = getBit(get_sign(value_1) + get_sign(value_2), 0);
    int cur_exp = get_exp(value_1) - get_exp(value_2);
    s21_decimal cpy_1 = value_1;
    cpy_1.bits[3] = 0;
    s21_decimal cpy_2 = value_2;
    cpy_2.bits[3] = 0;
    int increased = 0;
    while (s21_is_less(cpy_1, cpy_2) && (!is_zero(cpy_1)))
    {
      increase_exponent(&cpy_1);
      cpy_1.bits[3] = 0;
      increased++;
    }
    value_1.bits[0] = cpy_1.bits[0];
    value_1.bits[1] = cpy_1.bits[1];
    value_1.bits[2] = cpy_1.bits[2];
    value_1.bits[3] =
        (get_sign(value_1) << 31 | ((get_exp(value_1) + increased) << 16));
    cur_exp = remainder_div(value_1, value_2, result, 1);
    result->bits[3] |= (cur_sgn << 31 | cur_exp << 16);
  }
  return res;
}

int div_by_10(s21_decimal value, s21_decimal *result, int carry)
{
  unsigned long buf1 = 0;
  unsigned long buf2 = 0;

  result->bits[2] = ((unsigned long)value.bits[2] + carry * ((unsigned long)__UINT32_MAX__ + 1)) / 10;
  buf1 = (((unsigned long)value.bits[2] + carry * ((unsigned long)__UINT32_MAX__ + 1)) % 10) * ((unsigned long)__UINT32_MAX__ + 1);

  result->bits[1] = (((unsigned long)value.bits[1] + buf1) / 10);
  buf2 = (((unsigned long)value.bits[1] + buf1) % 10) * ((unsigned long)__UINT32_MAX__ + 1);

  result->bits[0] = (((unsigned long)value.bits[0] + buf2) / 10);
  return ((unsigned long)value.bits[0] + buf2) % 10;
}

int big_is_zero(s21_big_decimal value)
{
  return (!value.bits[0] && !value.bits[1] && !value.bits[2] && !value.bits[3] && !value.bits[4] && !value.bits[5]) ? 1 : 0;
}

int big_is_equal(s21_big_decimal value_1, s21_big_decimal value_2)
{
  if (value_1.bits[0] == value_2.bits[0] &&
      value_1.bits[1] == value_2.bits[1] &&
      value_1.bits[2] == value_2.bits[2] &&
      value_1.bits[3] == value_2.bits[3] &&
      value_1.bits[4] == value_2.bits[4] &&
      value_1.bits[5] == value_2.bits[5])
    return 1;
  else
    return 0;
}

int remainder_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result,
                  int precise)
{
  int q = 1;
  int cur_exp = get_exp(value_1) - get_exp(value_2);
  int max_shift = 0;
  for (int i = 95; i >= 0 && !max_shift; i--)
  {
    if (getBit(value_2.bits[i / 32], i % 32))
    {
      max_shift = 96 - i;
    }
  }
  *result = (s21_decimal){{0, 0, 0, 0}};
  value_1.bits[3] = 0;
  value_2.bits[3] = 0;
  s21_decimal tmp = (s21_decimal){{0, 0, 0, 0}};
  while (s21_is_greater_or_equal(value_1, value_2))
  {
    s21_decimal value_to_add = value_2;
    while (s21_is_greater_or_equal(value_1, value_to_add) && q < max_shift &&
           !getBit(result->bits[2], 31))
    { // b*2^q<=a
      value_to_add = value_2;
      tmp = value_to_add;
      for (int i = 0; i < q && !(getBit(tmp.bits[2], 30)); i++)
      {
        tmp = value_to_add;
        s21_add(value_to_add, value_to_add, &value_to_add);
      }
      q++;
    }
    q = q - 2;
    value_to_add = (s21_decimal){{0, 0, 0, 0}};
    value_to_add.bits[q / 32] = (1 << (q % 32));
    s21_add(*result, value_to_add, result);
    basic_sub(value_1, tmp, &value_1, &q);
    q = 0;
    while (precise && !(is_zero(value_1)) && s21_is_less(value_1, value_2) &&
           (cur_exp < 29) && (result->bits[2] <= 4294967295 / 10))
    {
      basic_mul(value_1, (s21_decimal){{10, 0, 0, 0}}, &value_1);
      if (!s21_is_equal(value_2, (s21_decimal){{10, 0, 0, 0}}))
      {
        basic_mul(*result, (s21_decimal){{10, 0, 0, 0}}, result);
      }
      cur_exp++;
    }
  }
  if (cur_exp == 29)
  {
    banking_rounding(result, 1);
    cur_exp--;
  }
  result->bits[3] = 0;
  return cur_exp;
}

void printBits(size_t const size, void const *const ptr)
{
  unsigned char *b = (unsigned char *)ptr;
  unsigned char byte;
  int i, j;

  for (i = size - 1; i >= 0; i--)
  {
    for (j = 7; j >= 0; j--)
    {
      byte = (b[i] >> j) & 1;
      printf("%u", byte);
    }
  }
  printf("\n");
}

void set_bit(unsigned int *destination, unsigned int position,
             unsigned int value)
{
  if (value)
    *destination |= (1 << position);
  else
    *destination &= ~(1 << position);
}

int is_zero(s21_decimal value)
{
  return (!value.bits[0] && !value.bits[1] && !value.bits[2]) ? 1 : 0;
}

void decimal_to_big_decimal(s21_decimal from, s21_big_decimal *to)
{
  for (int i = 0; i < 3; i++)
  {
    to->bits[i] = from.bits[i];
  }
  to->bits[3] = 0;
  to->bits[4] = 0;
  to->bits[5] = 0;
  to->exp = get_exp(from);
}

void shift_right(s21_decimal *value)
{
  int store_bit = 0;
  for (int i = 2; i >= 0; i--)
  {
    int new_bit = getBit(value->bits[i], 0);
    value->bits[i] = value->bits[i] >> 1;
    set_bit(&(value->bits[i]), 31, store_bit);
    store_bit = new_bit;
  }
}

void shift_big_decimal_left(s21_big_decimal *value)
{
  int store_bit = 0;
  int new_bit = 0;
  for (int i = 0; i < 6; i++)
  {
    new_bit = getBit(value->bits[i], 31);
    value->bits[i] = value->bits[i] << 1;
    set_bit(&(value->bits[i]), 0, store_bit);
    store_bit = new_bit;
  }
}

int add_for_multiply(s21_big_decimal value_1, s21_big_decimal value_2,
                     s21_big_decimal *result)
{
  int check = 0;
  int store_bit = 0;
  for (int i = 0; i < 192; i++)
  {
    int a = getBit(value_2.bits[i / 32], i % 32);
    int b = getBit(value_1.bits[i / 32], i % 32);
    set_bit(&(result->bits[i / 32]), i % 32, a ^ b ^ store_bit);
    store_bit = (a && b) || (b && store_bit) || (a && store_bit);
  }
  if (store_bit)
    check = 1;
  return check;
}

int multiply_10_big(s21_big_decimal *src)
{
  s21_big_decimal result = {{0}, 0};
  s21_decimal value_2 = {{10, 0, 0, 0}};
  int check = 0;
  s21_big_decimal tmp_1 = *src;
  while (!is_zero(value_2) && !check)
  {
    if (getBit(value_2.bits[0], 0))
      check = add_for_multiply(result, tmp_1, &result);
    shift_big_decimal_left(&tmp_1);
    if (getBit(result.bits[5], 31))
      check = 1;
    shift_right(&value_2);
  }
  result.exp = src->exp;
  *src = result;
  return check;
}

int normalize_big(s21_big_decimal *x1, s21_big_decimal *x2)
{
  int check = 0;
  s21_big_decimal *x_fix;
  int diff;
  if (x1->exp <= x2->exp)
  {
    x_fix = x1;
    diff = x2->exp - x1->exp;
  }
  else
  {
    x_fix = x2;
    diff = x1->exp - x2->exp;
  }
  while (diff--)
  {
    check = multiply_10_big(x_fix);
    if (check)
      break;
    x_fix->exp += 1;
  }
  return check;
}

int s21_is_equal(s21_decimal value_1, s21_decimal value_2)
{
  if (value_1.bits[0] == value_2.bits[0] &&
      value_1.bits[1] == value_2.bits[1] &&
      value_1.bits[2] == value_2.bits[2] &&
      get_sign(value_1) == get_sign(value_2) &&
      get_exp(value_1) == get_exp(value_2))
    return 1;
  else
    return 0;
}

int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2)
{
  return !s21_is_equal(value_1, value_2);
}

int s21_is_greater(s21_decimal value_1, s21_decimal value_2)
{
  s21_big_decimal value_1_big, value_2_big;

  decimal_to_big_decimal(value_1, &value_1_big);
  decimal_to_big_decimal(value_2, &value_2_big);

  normalize_big(&value_1_big, &value_2_big);

  int res = 0;
  if (is_zero(value_1) && is_zero(value_2))
  {
    res = 0;
  }
  else if (get_sign(value_1) < get_sign(value_2))
  {
    res = 1;
  }
  else if (get_sign(value_1) > get_sign(value_2))
  {
    res = 0;
  }
  else if (s21_is_equal(value_1, value_2))
  {
    res = 0;
  }
  else
  {
    for (int i = 191; i >= 0; i--)
    {
      if (getBit(value_1_big.bits[i / 32], i % 32) >
          getBit(value_2_big.bits[i / 32], i % 32))
      {
        res = 1;
        break;
      }
      else if (getBit(value_1_big.bits[i / 32], i % 32) <
               getBit(value_2_big.bits[i / 32], i % 32))
      {
        res = 0;
        break;
      }
    }
    if (get_sign(value_1) && get_sign(value_2))
      res = res ? 0 : 1;
  }
  return res;
}

int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2)
{
  int res = 0;
  if (s21_is_greater(value_1, value_2) || s21_is_equal(value_1, value_2))
  {
    res = 1;
  }
  return res;
}

int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2)
{
  int res = 0;
  if (!s21_is_greater(value_1, value_2))
  {
    res = 1;
  }
  return res;
}

int s21_is_less(s21_decimal value_1, s21_decimal value_2)
{
  int res = 0;
  if (!s21_is_greater(value_1, value_2) && !s21_is_equal(value_1, value_2))
  {
    res = 1;
  }
  return res;
}

void nulify_dec(s21_decimal *dst)
{
  dst->bits[0] = 0;
  dst->bits[1] = 0;
  dst->bits[2] = 0;
  dst->bits[3] = 0;
}

int s21_from_int_to_decimal(int src, s21_decimal *dst)
{
  nulify_dec(dst);
  if (src & 1 << 31)
  {
    dst->bits[3] |= 1 << 31;
    dst->bits[0] |= ~src + 1;
  }
  else
  {
    dst->bits[0] = src;
  }
  return 0;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst)
{
  int ret = 0;
  while (get_exp(src))
  {
    decrease_exponent(&src);
  }
  if (src.bits[0] & 1 << 31 || src.bits[1] || src.bits[1] & 1 << 31 ||
      src.bits[2] || src.bits[2] & 1 << 31)
  {
    ret = 1;
  }
  else
  {
    *dst = src.bits[0];
    if (get_sign(src))
    {
      *dst *= -1;
    }
  }
  return ret;
}

int count_digits(long n)
{
  int count = 0;
  if (n < 0)
  {
    n *= -1;
  }
  while (n >= 1)
  {
    n /= 10;
    count++;
  }
  return count;
}

int count_pre_null(float n)
{
  int i = 0;
  if ((int)n == 0)
  {
    for (i = 1; i < 9 && (int)(n * pow(10, i)) == 0; i++)
      ;
  }
  return i - 1;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst)
{
  if (fabs(src) < 1e-28 || src > MAX_DEC_VALUE || isinf(src) != 0)
  {
    return 1;
  }
  nulify_dec(dst);
  if (src < 0)
  {
    dst->bits[3] |= 1 << 31;
  }
  long tmp = (long)src;
  src -= tmp;
  int exp = 7 - count_digits(tmp);
  if (exp < 0)
  {
    while (count_digits(tmp) > 7)
    {
      tmp /= 10;
    }
    exp = 0;
  }
  else
  {
    src *= pow(10, exp - count_pre_null(src));
    tmp *= pow(10, exp - count_pre_null(src) - 1);
  }
  tmp += src;
  if (tmp < 0)
  {
    tmp = ~tmp + 1;
  }
  dst->bits[0] = tmp;
  dst->bits[3] |= (exp << 16);
  return 0;
}

void dec_to_str(s21_decimal src, char *str)
{
  int count = 0;
  if (get_sign(src))
  {
    str[0] = '-';
    count++;
  }
  for (int i = 2; i >= 0; i--)
  {
    for (int j = count_digits(src.bits[i]) - 1; j >= 0; j--)
    {
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

int s21_from_decimal_to_float(s21_decimal src, float *dst)
{
  *dst = src.bits[2];
  *dst *= pow(10, count_digits(src.bits[2]));
  *dst += src.bits[1];
  *dst *= pow(10, count_digits(src.bits[1]));
  *dst += src.bits[0];
  *dst /= pow(10, get_exp(src));
  return 0;
}
