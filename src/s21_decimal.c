#include "s21_decimal.h"

int s21_setBit(int num, int pos) { return (num | (1 << pos)); }

int s21_clearBit(int num, int pos) { return (num & (~(1 << pos))); }

int s21_toggleBit(int num, int pos) { return (num ^ (1 << pos)); }

int s21_get_exp(s21_decimal value) { return (value.bits[3] << 10) >> 26; }

int s21_get_sign(s21_decimal value) { return (value.bits[3] >> 31) & 1; }

int s21_get_bit(s21_decimal value, int index) {
  int block = index / 32;
  int block_index = index % 32;
  return (value.bits[block] & (1 << block_index)) >> block_index;
}


