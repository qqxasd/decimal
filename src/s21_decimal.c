#include "s21_decimal.h"

int main() {
  s21_decimal result;
  s21_add((s21_decimal){{0x00000001, 0x60000001, 0x00000000, 0x800E0000}},
          (s21_decimal){{0x00000004, 0x10000008, 0x00000000, 0x800F0000}},
          &result);
  printf("%d %d %d", result.bits[0], result.bits[1], result.bits[2]);
}

int setBit(int num, int pos) { return (num | (1 << pos)); }

int getBit(int num, int pos) { return ((num & (1 << pos)) >> pos); }

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
    s21_decimal tmp;
    int flag = 0;
    for (int i = 0; i < 10 && !flag; i++) {
      if (s21_add(*value_to_increase, *value_to_increase, &tmp)) {
        flag = 1;
      }
    }
    if (!flag) {
      *value_to_increase = tmp;
      (*value_to_increase).bits[3] += 65536;
    }
  }
}

void carry_value(s21_decimal *value, int bit_num_from) {
  (*value).bits[bit_num_from] -= 268435456;
  (*value).bits[bit_num_from + 1]++;
}

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  *result = (s21_decimal){{0x00000000, 0x00000000, 0x00000000, 0x00000000}};
  for (int i = 0; i < 96; i++) {
    int bit1 = getBit(value_1.bits[i / 32], i % 32);
    int bit2 = getBit(value_2.bits[i / 32], i % 32);
    if (bit1 | bit2) {
      (*result).bits[i / 32] = setBit((*result).bits[i / 32], i % 32);
    } else {
      (*result).bits[i / 32] = clearBit((*result).bits[i / 32], i % 32);
    }
  }
  (*result).bits[3] = value_2.bits[3];
  return 0;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {}

void basic_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result,
               int *res) {}

void basic_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result,
               int *res, int *change_sign) {}