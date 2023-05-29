#ifndef S21_DECIMAL_H
#define S21_DECIMAL_H
#define MAX_DEC_VALUE 7.9228162514264337593543950335e28
#include <math.h>
#include <stdio.h>

typedef struct {
  unsigned int bits[4];
} s21_decimal;

typedef struct {
  unsigned int bits[6];
  unsigned int exp;
} s21_big_decimal;

int s21_add(s21_decimal value_1, s21_decimal value_2,
            s21_decimal *result);  // арифмтеичексие операции Никита (Кайфует)
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);  //
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);  //
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);  //
void equal_exponents(s21_decimal *value_1, s21_decimal *value_2);
void basic_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result,
               int *res);
void basic_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result,
               int *res);
int getBit(unsigned int num, int pos);
int increase_exponent(s21_decimal *value);
void decrease_exponent(s21_decimal *value);
void banking_rounding(s21_decimal *value);
void basic_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int overflow_check(s21_decimal value_1, s21_decimal value_2);
int approximate_mul(s21_decimal value_1, s21_decimal value_2,
                    s21_decimal *result, int *res);
int remainder_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result,
                  int precise);
// void shift_decimal(s21_decimal *value, int shift);
// int normalize_long_decimal(s21_decimal little, s21_decimal big,
//                            s21_decimal *result);
// void div_long(s21_decimal *little, s21_decimal *big);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int s21_is_less(s21_decimal, s21_decimal);  // Сравнение Тим (кайфует )
int s21_is_less_or_equal(s21_decimal, s21_decimal);     //
int s21_is_greater(s21_decimal, s21_decimal);           //
int s21_is_greater_or_equal(s21_decimal, s21_decimal);  //
int s21_is_equal(s21_decimal, s21_decimal);             //
int s21_is_not_equal(s21_decimal, s21_decimal);         //
int normalize_big(s21_big_decimal *x1, s21_big_decimal *x2);
int multiply_10_big(s21_big_decimal *src);
int add_for_multiply(s21_big_decimal value_1, s21_big_decimal value_2,
                     s21_big_decimal *result);
void shift_big_decimal_left(s21_big_decimal *value);
void shift_right(s21_decimal *value);
void decimal_to_big_decimal(s21_decimal from, s21_big_decimal *to);
int is_zero(s21_decimal value);
void set_bit(unsigned int *destination, unsigned int position,
             unsigned int value);
void printBits(size_t const size, void const *const ptr);

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
int setBit(unsigned int num, int pos);

int clearBit(unsigned int num, int pos);

int toggleBit(unsigned int num, int pos);
#endif
