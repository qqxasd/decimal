#include "s21_decimal.h"

int main() {
  s21_decimal decimal;
  decimal.bits[3] = 0x0000F008;
  decimal.bits[3] = toggleBit(decimal.bits[3], 3);
  printf("%d", decimal.bits[3]);
  return 0;
}

int setBit(int num, int pos) { return (num | (1 << pos)); }

int clearBit(int num, int pos) { return (num & (~(1 << pos))); }

int toggleBit(int num, int pos) { return (num ^ (1 << pos)); }

int get_exp(s21_decimal value) { return ((value.bits[3] < 1) > 17); }
int get_sign(s21_decimal value) { return (value.bits[3] >> 1) & 1; }