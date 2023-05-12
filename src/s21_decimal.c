#include "s21_decimal.h"

int main() {
  s21_decimal decimal;
  decimal.bits[3] = 0x801C0000;
  printf("%d %d %d", decimal.bits[3], get_exp(decimal), decimal.bits[3]);
  return 0;
}

int setBit(int num, int pos) { return (num | (1 << pos)); }

int clearBit(int num, int pos) { return (num & (~(1 << pos))); }

int toggleBit(int num, int pos) { return (num ^ (1 << pos)); }

int get_exp(s21_decimal value) { return (value.bits[3] << 10) >> 26; }
int get_sign(s21_decimal value) { return (value.bits[3] >> 31) & 1; }