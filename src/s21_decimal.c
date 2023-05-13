#include "s21_decimal.h"

int main() {
  s21_decimal decimal1, decimal2, result, result2;
  decimal1 = (s21_decimal){{0x00000000, 0x0FFFFFFF, 0x7FFFFFFF, 0x00100000}};
  decimal2 = (s21_decimal){{0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFF1, 0x00090000}};
  int res = s21_add(decimal1, decimal2, &result);
  printf("%d %d %d %d*10^(-%d)\n%d\n", get_sign(result), result.bits[2],
         result.bits[1], result.bits[0], get_exp(result), res);
  s21_sub(decimal1, decimal2, &result2);
  printf("%d %d %d %d*10^(-%d)\n", get_sign(result2), result2.bits[2],
         result2.bits[1], result2.bits[0], get_exp(result2));
  return 0;
}

int setBit(int num, int pos) { return (num | (1 << pos)); }

int clearBit(int num, int pos) { return (num & (~(1 << pos))); }

int toggleBit(int num, int pos) { return (num ^ (1 << pos)); }

int get_exp(s21_decimal value) { return (value.bits[3] << 10) >> 26; }
int get_sign(s21_decimal value) { return (value.bits[3] >> 31) & 1; }

int cant_carry_bit(int num) { return (num < 268435456); }

void borrow_from_higher(s21_decimal *value, s21_decimal *base,
                        int bit_num_from) {
  (*base).bits[bit_num_from - 1] += 268435456;
  (*value).bits[bit_num_from]--;
}

void equal_exponents(int exp_lower, int exp_higher,
                     s21_decimal *value_to_increase) {
  int diff = exp_higher - exp_lower;
  for (int i = 0; i < diff; i++) {
    int tmp = (*value_to_increase).bits[0] * 10;
    printf("%d\n", tmp / (*value_to_increase).bits[0]);
    (*value_to_increase).bits[0] *= 10;
    if ((*value_to_increase).bits[0] < 0) {
      // to_lower_exponent();
    }
    (*value_to_increase).bits[3] += 65536;
  }
}

void carry_value(s21_decimal *value, int bit_num_from) {
  (*value).bits[bit_num_from] -= 268435456;
  (*value).bits[bit_num_from + 1]++;
}

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = 0;
  int change_sign = 0;
  int exp1 = get_exp(value_1);
  int exp2 = get_exp(value_2);
  int sign1 = get_sign(value_1);
  int sign2 = get_sign(value_2);
  if (exp1 > exp2) {
    equal_exponents(exp2, exp1, &value_2);
  } else if (exp2 > exp1) {
    equal_exponents(exp1, exp2, &value_1);
  }
  if (sign1 == sign2) {
    basic_add(value_1, value_2, result, &res);
  }
  if (sign1 > sign2) {
    value_1.bits[3] = toggleBit(value_1.bits[3], 31);
    basic_sub(value_2, value_1, result, &res, &change_sign);
  }
  if (sign2 > sign1) {
    value_1.bits[3] = toggleBit(value_1.bits[3], 31);
    basic_sub(value_1, value_2, result, &res, &change_sign);
  }
  if (change_sign) {
    (*result).bits[3] = toggleBit((*result).bits[3], 31);
  }
  return res;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = 0;
  int exp1 = get_exp(value_1);
  int exp2 = get_exp(value_2);
  int change_sign = 0;
  if (exp1 > exp2) {
    equal_exponents(exp2, exp1, &value_2);
  } else if (exp2 > exp1) {
    equal_exponents(exp1, exp2, &value_1);
  }
  basic_sub(value_1, value_2, result, &res, &change_sign);
  if (change_sign) {
    (*result).bits[3] = toggleBit((*result).bits[3], 31);
  }
  return res;
}

void basic_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result,
               int *res) {
  int carry_flag = 0;
  (*result).bits[0] = value_1.bits[0] + value_2.bits[0];
  if (value_1.bits[0] >= 0 && value_2.bits[0] >= 0 && (*result).bits[0] < 0) {
    carry_flag = 1;
    (*result).bits[0] = clearBit((*result).bits[0], 31);
  }
  (*result).bits[1] = value_1.bits[1] + value_2.bits[1] + carry_flag;
  carry_flag = 0;
  if (value_1.bits[1] >= 0 && value_2.bits[1] >= 0 && (*result).bits[1] < 0) {
    carry_flag = 1;
    (*result).bits[1] = clearBit((*result).bits[1], 31);
  }
  (*result).bits[2] = value_1.bits[2] + value_2.bits[2] + carry_flag;
  carry_flag = 0;
  if (value_1.bits[2] >= 0 && value_2.bits[2] >= 0 && (*result).bits[2] < 0) {
    carry_flag = 1;
    (*result).bits[2] = clearBit((*result).bits[2], 31);
  }
  (*result).bits[3] = value_1.bits[3] - carry_flag * 65536;
  if (get_exp(*result) < 0) {
    *res = 1;
    (*result) = (s21_decimal){{0, 0, 0, 0}};
  }
}

void basic_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result,
               int *res, int *change_sign) {
  int carry_flag = 0;
  int flag = 0;
  (*result).bits[2] = value_1.bits[2] - value_2.bits[2];
  if ((*result).bits[2] < 0) {
    if (cant_carry_bit(value_1.bits[1])) {
      if (cant_carry_bit(value_1.bits[0])) {
        *change_sign = 1;
        basic_sub(value_2, value_1, result, res, change_sign);
        flag = 1;
      } else {
        carry_value(&value_1, 0);
        carry_value(&value_1, 1);
      }
    } else {
      carry_value(&value_1, 1);
    }
    (*result).bits[2] = value_1.bits[2] - value_2.bits[2];
  }
  if (!flag) {
    (*result).bits[1] = value_1.bits[1] - value_2.bits[1];
    if ((*result).bits[1] < 0) {
      if ((*result).bits[2] > 0) {
        while ((*result).bits[2] > 0 &&
               ((*result).bits[1] = (value_1.bits[1] - value_2.bits[1])) < 0) {
          borrow_from_higher(result, &value_1, 2);
        }
      } else {
        if (cant_carry_bit(value_1.bits[0])) {
          *change_sign = 1;
          basic_sub(value_2, value_1, result, res, change_sign);
          flag = 1;
        } else {
          carry_value(&value_1, 0);
        }
        (*result).bits[1] = value_1.bits[1] - value_2.bits[1];
      }
    }
    if (!flag) {
      (*result).bits[0] = value_1.bits[0] - value_2.bits[0];
      if ((*result).bits[0] < 0) {
        while (((*result).bits[0] = (value_1.bits[0] - value_2.bits[0])) < 0 &&
               ((*result).bits[2] > 0 || (*result).bits[1] > 0)) {
          if ((*result).bits[1] == 0 && (*result).bits[2] > 0) {
            borrow_from_higher(result, result, 2);
          }
          borrow_from_higher(result, &value_1, 1);
        }
      }
      if ((*result).bits[2] == 0 && (*result).bits[1] == 0) {
        *change_sign = 1;
        (*result).bits[0] = value_2.bits[0] - value_1.bits[0];
      }
      (*result).bits[3] = value_1.bits[3] + carry_flag * 65536;
      if (get_exp(*result) < 0) {
        *res = 1;
        (*result) = (s21_decimal){{0, 0, 0, 0}};
      }
    }
  }
}