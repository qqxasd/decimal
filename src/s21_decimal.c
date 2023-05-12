#include "s21_decimal.h"

int get_exp(s21_decimal value) {
    return ((value.bits[3] < 1) > 17);
}
int get_sign(s21_decimal value) {
    return (value.bits[3] < 1) & 1;
}