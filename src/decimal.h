#ifndef DECIMAL_H
#define DECIMAL_H
#define MAX_DEC_VALUE 7.9228162514264337593543950335e28
#include <math.h>
#include <stdio.h>

typedef struct {
  unsigned int bits[4];
} decimal;

typedef struct {
  unsigned int bits[6];
  unsigned int exp;
} big_decimal;

int add(decimal value_1, decimal value_2,
        decimal *result);  // арифмтеичексие операции Никита (Кайфует)
int sub(decimal value_1, decimal value_2, decimal *result);  //
int mul(decimal value_1, decimal value_2, decimal *result);  //
int div(decimal value_1, decimal value_2, decimal *result);  //
void equal_exponents(decimal *value_1, decimal *value_2);
int basic_add(decimal value_1, decimal value_2, decimal *result, int *res);
void basic_sub(decimal value_1, decimal value_2, decimal *result, int *res);
int getBit(unsigned int num, int pos);
int increase_exponent(decimal *value);
int mul_by_10(decimal *value);
int big_mul_by_10(big_decimal *value);
int decrease_exponent(decimal *value);
void basic_mul(decimal value_1, decimal value_2, decimal *result);
int overflow_check(decimal value_1, decimal value_2);
int remainder_div(decimal value_1, decimal value_2, decimal *result);
void shift(decimal *value);
void big_shift(big_decimal *value);
void big_add(big_decimal value_1, big_decimal value_2, big_decimal *result);
void big_mul(big_decimal value_1, big_decimal value_2, big_decimal *result);
int big_div_by_10(big_decimal value, big_decimal *result);
int long_mul(decimal value_1, decimal value_2, decimal *result);
int big_decimal_to_decimal(big_decimal long_result, decimal *result);
void remainder_big_div(big_decimal value_1, big_decimal value_2,
                       big_decimal *result);
int div_by_10(decimal value, decimal *result, int carry);
void round_after_division(decimal *result, int *cur_exp, int max_shift,
                          decimal value_1, decimal value_2);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int is_less(decimal, decimal);  // Сравнение Тим (кайфует )
int is_less_or_equal(decimal, decimal);     //
int is_greater(decimal, decimal);           //
int is_greater_or_equal(decimal, decimal);  //
int is_equal(decimal, decimal);             //
int is_not_equal(decimal, decimal);         //
int normalize_big(big_decimal *x1, big_decimal *x2);
int multiply_10_big(big_decimal *src);
int add_for_multiply(big_decimal value_1, big_decimal value_2,
                     big_decimal *result);
void shift_big_decimal_left(big_decimal *value);
void shift_right(decimal *value);
void decimal_to_big_decimal(decimal from, big_decimal *to);
int is_zero(decimal value);
void set_bit(unsigned int *destination, unsigned int position,
             unsigned int value);

int get_bit(decimal value, int index);
void print_decmal(decimal value);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int from_int_to_decimal(int src,
                        decimal *dst);  // Переводы Денис (двигается на блессе)
int from_float_to_decimal(float src, decimal *dst);  //
int from_decimal_to_int(decimal src, int *dst);      //
int from_decimal_to_float(decimal src, float *dst);  //

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int d_floor(decimal value,
            decimal *result);  // Дополнительные фукнции Санан (радуется
                               // что дописал принтф)
int d_round(decimal value, decimal *result);  // кайфует нереально!!!
int truncate(decimal value,
             decimal *result);  // реально самый счастливый если успеет
                                // дописать принтф
int negate(decimal value, decimal *result);  // )))))

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int get_exp(decimal value);
int get_sign(decimal value);
int setBit(unsigned int num, int pos);

int clearBit(unsigned int num, int pos);

int toggleBit(unsigned int num, int pos);

#endif
