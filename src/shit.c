#include "s21_decimal.h"


int s21_negate(s21_decimal value, s21_decimal *result){
    // Возвращает результат умножения указанного Decimal на -1.
    if(get_sign(value)) clearBit(((*result).bits[3]), 31);
    else setBit(((*result).bits[3]), 31);
    return 0;
}


int s21_truncate(s21_decimal value, s21_decimal *result) {
    // Возвращает целые цифры указанного Decimal числа; любые дробные цифры отбрасываются, включая конечные нули.
    int exp = get_exp(value);
    if(get_sign(value)) setBit((*result).bits[3], 31);
    int tmp;
    for (int i = 0; i < 3 * 32; i++) {
        tmp = getBit(value.bits[i/32], i % 32);
        if(tmp) setBit((*result).bits[i/32], i % 32);
    }
    for (int i = 0; i < exp; i ++) {
        divide_by_ten(result);
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
        if ((*result).bits[0] > (*result).bits[0]++) {
            // obrabotka perepolneniya
        }
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
            if ((*result).bits[0] > (getBit((*result).bits[0], 0))? ((*result).bits[0]++) : ((*result).bits[0])) {
                // TERNARNIKKK, smotrim posledniy bit chisla,
                // esli on est -> chislo nechetnoe -> chislo ++
                // elsi ego net -> chislo chetnoe -> pass
                // jbrabotka perepolneniya
            }
        }
        else if(remainder > 5) {
            if ((*result).bits[0] > (*result).bits[0]++) {
                // jbrabotka perepolneniya
            }
        }
    }
    return 0;
}

int divide_by_ten(s21_decimal *value) {
    s21_decimal remainder; // kak ego obyavlyat???
    int bit;
    for (int i = 0; i < 3 * 32; i++) {
        bit = getBit((*value).bits[i/32], i % 32);
        if(bit) setBit(remainder.bits[i/32], i % 32);
    }
    s21_from_int_to_decimal(0, value);

    s21_decimal tmp; // kak ego obyavlyat??? 
    s21_from_int_to_decimal(1, &tmp); // 1 v decimale


    s21_decimal ten; // kak ego obyavlyat??? 
    s21_from_int_to_decimal(10, &ten); // 10 v decimale
    while (s21_is_less(ten, remainder)) {
        while (s21_is_less(ten, remainder)) {
            shift_decimal(&ten, 1);
            shift_decimal(&tmp, 1);
        }
        s21_add(*value, ten, value);
        s21_sub(remainder, tmp, &remainder);
        s21_from_int_to_decimal(1, &tmp);
        s21_from_int_to_decimal(10, &ten);
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