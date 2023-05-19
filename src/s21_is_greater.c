#include "s21_decimal.h"

int s21_is_greater(s21_decimal value_1, s21_decimal value_2) {
  s21_decimal *value_1_tmp, *value_2_tmp;
  int return_value = 0;
  int negative_value_1 = get_sign(value_1);
  int negative_value_2 = get_sign(value_2);


  int tmp_raz = 0;
  int bit_1 = 0;
  int bit_2 = 0;
  int value_1_flag = 1;
  int end_loop_flag = 0;

  if (is_zero(value_1) && !negative_value_2) {
    return_value = 0;
  } else if (negative_value_1 == 1 && negative_value_2 == 0) {
    return_value = 0;
  } else if (negative_value_1 == 0 && negative_value_2 == 1) {
    return_value = 1;
  } else {
    if (get_exp(value_1) <= get_exp(value_2)) {
      value_1_tmp = &value_1;
      value_2_tmp = &value_2;
    } else {
      value_1_flag = 0;
      value_1_tmp = &value_2;
      value_2_tmp = &value_1;
    }
    tmp_raz = get_exp(*value_1_tmp) - get_exp(*value_2_tmp);

    for (int i = 95; i >= tmp_raz && !end_loop_flag; --i) {
      if (i < 0) {
        bit_1 = 0;
      } else {
        bit_1 = get_bit(*value_1_tmp, i);
      }
      if (i - tmp_raz > 95) {
        bit_2 = 0;
      } else {
        bit_2 = get_bit(*value_2_tmp, i - tmp_raz);
      }
      if (bit_1 > bit_2) {
        if (value_1_flag) {
          return_value = 1;
        } else {
          return_value = 0;
        }
        end_loop_flag = 1;
      } else if (bit_1 < bit_2) {
        if (value_1_flag) {
          return_value = 0;
        } else {
          return_value = 1;
        }
        end_loop_flag = 1;
      }
    }

    if (negative_value_1 && negative_value_2) {
      return_value = !return_value;
    }
  }
  return return_value;
}
