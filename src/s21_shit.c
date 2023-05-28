#include "s21_decimal.h"


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
        // divide_by_ten(result);
        s21_div(*result, ten, result);
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
        remainder = divide_by_ten(result);
    }

    if(remainder != 0 && get_sign(value)) {
        s21_decimal one = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}; // 1
        s21_add(*result, one, result);
        // if ((*result).bits[0] > (*result).bits[0]++) {
        //     // obrabotka perepolneniya
        // }
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
        remainder = divide_by_ten(result);
    }

    if(remainder != 0) { 
        if(remainder == 5) { // tut mb po raznomu okruglyautsya oricatelnie chisla 
            if(getBit((*result).bits[0], 0)){ 
                s21_decimal one = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}; // 1
                s21_add(*result, one, result); 
            }
            // if ((*result).bits[0] > (getBit((*result).bits[0], 0))? ((*result).bits[0]++) : ((*result).bits[0])) {
            //     // TERNARNIKKK, smotrim posledniy bit chisla,
            //     // esli on est -> chislo nechetnoe -> chislo ++
            //     // elsi ego net -> chislo chetnoe -> pass
            //     // jbrabotka perepolneniya
            // }
        }
        else if(remainder > 5) {
            s21_decimal one = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}; // 1
            s21_add(*result, one, result);
        }
    }
    return 0;
}

int divide_by_ten(s21_decimal *value) { // mojno ispolzovat dlya lubogo deleniya
    s21_decimal remainder = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}; // kak ego obyavlyat???
    int bit = 0;
    for (int i = 0; i < 3 * 32; i++) {
        bit = getBit((*value).bits[i/32], i % 32);
        if(bit) remainder.bits[i/32] = setBit(remainder.bits[i/32], i % 32);
    }
    // s21_from_int_to_decimal(0, value);
    for (int i = 0; i < 3 * 32; i++) {
        (*value).bits[i/32] = clearBit(value->bits[i/32], i % 32);
    }

    s21_decimal tmp = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}; // 1


    s21_decimal ten = {{0x0000000A, 0x00000000, 0x00000000, 0x00000000}}; // 10
    while (s21_is_less(ten, remainder)) {
        while (s21_is_less(ten, remainder)) {
            s21_add(ten, ten,&ten);
            s21_add(tmp, tmp, &tmp);
            // shift_decimal(&ten, 1);
            // shift_decimal(&tmp, 1);
        }
        s21_add(*value, ten, value);
        s21_sub(remainder, tmp, &remainder);
        // s21_from_int_to_decimal(1, &tmp);
        // s21_from_int_to_decimal(10, &ten);
        for (int i = 0; i < 3 * 32; i++) {
        tmp.bits[i/32] = clearBit(tmp.bits[i/32], i % 32);
        }
        tmp.bits[0] = setBit(tmp.bits[0], 1);
        for (int i = 0; i < 3 * 32; i++) {
        ten.bits[i/32] = clearBit(ten.bits[i/32], i % 32);
        }
        ten.bits[0] = setBit(ten.bits[0], 2);
        ten.bits[0] = setBit(ten.bits[0], 4);
    }
    int res = remainder.bits[0];
    
    return res;
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