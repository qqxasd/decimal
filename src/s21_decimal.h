#ifndef S21_DECIMAL_H
#define S21_DECIMAL_H

#include <check.h>
#include <stdio.h>

typedef struct {
  int bits[4];
} s21_decimal;

int s21_add(s21_decimal value_1, s21_decimal value_2,
            s21_decimal *result);  // арифмтеичексие операции Никита (Кайфует)
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);  //
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);  //
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);  //
void equal_exponents(int exp1, int exp2, s21_decimal *value_to_increase);
void basic_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result,
               int *res);
void basic_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result,
               int *res, int *change_sign);
void carry_value(s21_decimal *value, int bit_num_from);
int cant_carry_bit(int num);
void borrow_from_higher(s21_decimal *value, s21_decimal *base,
                        int bit_num_from);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int s21_is_less(s21_decimal, s21_decimal);  // Сравнение Тим (кайфует )
int s21_is_less_or_equal(s21_decimal, s21_decimal);     //
int s21_is_greater(s21_decimal, s21_decimal);           //
int s21_is_greater_or_equal(s21_decimal, s21_decimal);  //
int s21_is_equal(s21_decimal, s21_decimal);             //
int s21_is_not_equal(s21_decimal, s21_decimal);         //

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

int get_exp(s21_decimal value);
int get_sign(s21_decimal value);
int setBit(int num, int pos);

int clearBit(int num, int pos);

int toggleBit(int num, int pos);

#endif