#include "s21_decimal.h"
int divide_by_ten(s21_decimal value, s21_decimal *result);

int s21_negate(s21_decimal value, s21_decimal *result){
    // Возвращает результат умножения указанного Decimal на -1.
    if(get_sign(value)) (*result).bits[3] = clearBit(((*result).bits[3]), 31);
    else (*result).bits[3] = setBit(((*result).bits[3]), 31);
    return 0;
}


int s21_truncate(s21_decimal value, s21_decimal *result) {
    // Возвращает целые цифры указанного Decimal числа; любые дробные цифры отбрасываются, включая конечные нули.
    int exp = get_exp(value);
    if(get_sign(value)) (*result).bits[3] = setBit((*result).bits[3], 31);
    int tmp;
    for (int i = 0; i < 3 * 32; i++) {
        tmp = getBit(value.bits[i/32], i % 32);
        if(tmp) (*result).bits[i/32] = setBit((*result).bits[i/32], i % 32);
    }
    // printf("new = %0x, %0x, %0x, %0x\n", (*result).bits[0], (*result).bits[1], (*result).bits[2], (*result).bits[3]);
    s21_decimal ten = {{0x0000000A, 0x00000000, 0x00000000, 0x00000000}}; // 10
    for (int i = 0; i < exp; i ++) {
        divide_by_ten(*result, result);
    }
    return 0;
}

int s21_floor(s21_decimal value, s21_decimal *result) {
    // Округляет указанное Decimal число до ближайшего целого числа в сторону отрицательной бесконечности.
    int exp = get_exp(value);
    if(get_sign(value)) setBit((*result).bits[3], 31);
    int bit;
    for (int i = 0; i < 3 * 32; i++) {
        bit = getBit(value.bits[i/32], i % 32);
        if(bit) setBit((*result).bits[i/32], i % 32);
    }
    int remainder;
    for (int i = 0; i < exp; i ++) {
        remainder = divide_by_ten(*result, result);
    }

    if(remainder != 0) {
        s21_decimal one = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}; // 1
        if(get_sign(value)) s21_add(*result, one, result);
        else s21_sub(*result, one, result);
    }
    return 0;
}


int s21_round(s21_decimal value, s21_decimal *result) {
    // Округляет Decimal до ближайшего целого числа.
    int exp = get_exp(value);
    if(get_sign(value)) setBit((*result).bits[3], 31);
    int bit;
    for (int i = 0; i < 3 * 32; i++) {
        bit = getBit(value.bits[i/32], i % 32);
        if(bit) setBit((*result).bits[i/32], i % 32);
    }
    int remainder;
    for (int i = 0; i < exp; i ++) {
        remainder = divide_by_ten(*result, result);
    }
    if(remainder > 5) {
        s21_decimal one = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}; // 1
        s21_add(*result, one, result);
    }
    return 0;
}

int divide_by_ten(s21_decimal value, s21_decimal *result) {
    unsigned long buf1;
    unsigned long buf2;
    result->bits[2] = value.bits[2] / 10;
    buf1 = value.bits[2] % 10 * (__UINT32_MAX__ + 1);
    result->bits[1] = ((unsigned long)value.bits[1] + buf1) / 10;
    buf2 = ((unsigned long)value.bits[1] + buf1) % 10 * (__UINT32_MAX__ + 1);
    result->bits[0] = ((unsigned long)value.bits[0] + buf2) / 10;
    return ((unsigned long)value.bits[0] + buf2) % 10;
}

// int setBit(unsigned int num, int pos) { return (num | (1 << pos)); }
// int getBit(unsigned int num, int pos) { return ((num & (1 << pos)) >> pos); }
// int clearBit(unsigned int num, int pos) { return (num & (~(1 << pos))); }
// int get_sign(s21_decimal value) { return (value.bits[3] >> 31) & 1; }
// int get_exp(s21_decimal value) { return (value.bits[3] << 10) >> 26; }

// int s21_is_less_or_equal(s21_decimal, s21_decimal)
// int s21_from_int_to_decimal
// void shift_decimal(s21_decimal *value, int shift);
// int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
// int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

// int s21_is_equal(s21_decimal, s21_decimal);