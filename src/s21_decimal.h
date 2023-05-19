#ifndef S21_DECIMAL_H
#define S21_DECIMAL_H

#include <check.h>
#include <stdio.h>

#define START_LOW_BIT 0
#define START_MID_BIT 32
#define START_HIGHT_BIT 64
#define START_SERVICE_BIT 96

typedef struct {
  int bits[4];
} s21_decimal;

int s21_add(s21_decimal value_1, s21_decimal value_2,
            s21_decimal *result);  // арифмтеичексие операции Никита (Кайфует)
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);  //
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);  //
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);  //

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int s21_is_less(s21_decimal, s21_decimal);  // Сравнение Тим (кайфует )
int s21_is_less_or_equal(s21_decimal, s21_decimal);     //
int s21_is_greater(s21_decimal, s21_decimal);           //
int s21_is_greater_or_equal(s21_decimal, s21_decimal);  //
int s21_is_equal(s21_decimal, s21_decimal);             //
int s21_is_not_equal(s21_decimal, s21_decimal);         //

int get_bit(s21_decimal value, int index);
int is_zero(s21_decimal value);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int s21_from_int_to_decimal(
    int src, s21_decimal *dst);  // Переводы Денис (двигается на блессе)
int s21_from_float_to_decimal(float src, s21_decimal *dst);  //
int s21_from_decimal_to_int(s21_decimal src, int *dst);      //
int s21_from_decimal_to_float(s21_decimal src, float *dst);  //

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int s21_floor(s21_decimal value,
              s21_decimal *result);  // Дополнительные фукнции Санан (радуется
                                     // что дописал принтф)
int s21_round(s21_decimal value, s21_decimal *result);  // кайфует нереально!!!
int s21_truncate(s21_decimal value,
                 s21_decimal *result);  // реально самый счастливый если успеет
                                        // дописать принтф
int s21_negate(s21_decimal value, s21_decimal *result);  // )))))

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int get_exp(
    s21_decimal value);  // при записи в виде шестнадцатеричного числа за
                         // экспоненту отвечает 3 и 4 цифры, т.е 0x00XX0000. При
                         // этом значения могут варьироваться от 0 до 28, т.е.
                         // от 0x00000000 до 0x001С0000
int get_sign(
    s21_decimal value);  // при записи в виде шестнадцатеричного числа при
                         // положительном знаке первое число будет 0, т.е
                         // 0x0XXXXXXX, а при отрицательном - 0x8XXXXXXX.
int setBit(int num, int pos);

int clearBit(int num, int pos);

int toggleBit(int num, int pos);
#endif