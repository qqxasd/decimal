
#include "../s21_decimal.h"
#include "unit_tests.h"

// VHODNIE DANNIE
static s21_decimal num1[] = {
    {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},  // 0 (0)
    {{0x00000001, 0x00000000, 0x00000000,
      0x800E0000}},  // -0.00000000000001 (1)
    {{0x00000019, 0x00000000, 0x00000000, 0x00010000}},  // 2.5 (2)
    {{0x00000183, 0x00000000, 0x00000000, 0x00020000}},  // 3.87 (3)
    {{0x14490831, 0x00048E23, 0x00000000,
      0x00070000}},  // 128218122.2172721 (4)
    {{0x00000001, 0x00000000, 0x00000000, 0x00000000}},  // 1 (5)
    {{0x77D5E3AA, 0x0000011C, 0x00000000, 0x00060000}},  // 1221781.218218 (6)
    {{0x80000000, 0x80000000, 0x80000000,
      0x801C0000}},  // -3.9614081266355540835774234624 (7)
    {{0x80000000, 0x80000000, 0x80000000,
      0x000E0000}},  // 396140812663555.40835774234624 (8)
    {{0x00000001, 0x00000000, 0x00000000,
      0x001C0000}},  // 0.0000000000000000000000000001 (9)
    {{0x000F4240, 0x00000000, 0x00000000,
      0x001C0000}},  // 0.0000000000000000000001000000 (10)
    {{0x80000000, 0x80000000, 0x80000000,
      0x00000000}},  // 39614081266355540835774234624 (11)
    {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
      0x00010000}},  // 7922816251426433759354395033.5 (12)
    {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
      0x00000000}},  // 79228162514264337593543950335 (13)
    {{0x1422CF6C, 0x000001FC, 0x00000000, 0x80030000}},  // -2182181212.012 (14)
    {{0x2564B1EA, 0x3927F1F3, 0x041C50D7,
      0x000B0000}},  // 12721717217217217.21821901290 (15)
    {{0x0001869F, 0x00000000, 0x00000000, 0x00050000}},  // 0.99999 (16)
    {{0x0098967E, 0x00000000, 0x00000000, 0x80060000}},  // -9.999998 (17)
    {{0x000000CF, 0x00000000, 0x00000000, 0x00010000}},  // 20.7 (18)
    {{0x0707E3C5, 0x002D8D61, 0x00000000,
      0x80090000}},  // -12821821.821281221 (19)
    {{0x846A4B4C, 0x0000012A, 0x00000000, 0x00080000}},  // 12821.21812812 (20)
    {{0x000035A3, 0x00000000, 0x00000000,
      0x00170000}},  // 0.00000000000000000013731 (21)
    {{0x79353447, 0x00000004, 0x00000000, 0x00010000}},  // 1921339911.1 (22)
    {{0xFFFFFFFD, 0xFFFFFFFF, 0xFFFFFFFF,
      0x00000000}},  // 79228162514264337593543950333 (23)
    {{0xfffffffe, 0xffffffff, 0xffffffff,
      0x00000000}},  // 79228162514264337593543950334 (24)
    {{0x00000005, 0x00000000, 0x00000000, 0x00010000}},  // 0.5 (25)
    {{0x00000033, 0x00000000, 0x00000000, 0x00020000}},  // 0.51 (26)
    {{0x00000031, 0x00000000, 0x00000000, 0x00020000}},  // 0.49 (27)
    {{0x00000005, 0x00000000, 0x00000000, 0x80010000}},  // -0.5 (28)
    {{0x00000033, 0x00000000, 0x00000000, 0x80020000}},  // -0.51 (29)
    {{0x00000031, 0x00000000, 0x00000000, 0x80020000}},  // -0.49 (30)
};

// RESULT OF TRUNCATE
static s21_decimal result_truncate[] = {
    {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},  // 0 (0)
    {{0x00000000, 0x00000000, 0x00000000, 0x80000000}},  //-0 (1)
    {{0x00000002, 0x00000000, 0x00000000, 0x00000000}},  // 2 (2)
    {{0x00000003, 0x00000000, 0x00000000, 0x00000000}},  // 4 (3)
    {{0x07a4740a, 0x00000000, 0x00000000, 0x00000000}},  // 128218122 (4)
    {{0x00000001, 0x00000000, 0x00000000, 0x00000000}},  // 1 (5)
    {{0x0012a495, 0x00000000, 0x00000000, 0x00000000}},  // 1221781 (6)
    {{0x00000003, 0x00000000, 0x00000000, 0x80000000}},  // -3  (7)
    {{0xb86b7b03, 0x00016849, 0x00000000, 0x00000000}},  // 396140812663555 (8)
    {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},  // 0 (9)
    {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},  // 0 (10)
    {{0x80000000, 0x80000000, 0x80000000,
      0x00000000}},  // 39614081266355540835774234624 (11)
    {{0x99999999, 0x99999999, 0x19999999,
      0x00000000}},  // 7922816251426433759354395033  (12)
    {{0xffffffff, 0xffffffff, 0xffffffff,
      0x00000000}},  // 79228162514264337593543950335  (13)
    {{0x8211715c, 0x00000000, 0x00000000, 0x80000000}},  // -2182181212 (14)
    {{0x9baa4ac1, 0x002d3255, 0x00000000,
      0x00000000}},  // 12721717217217217 (15)
    {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},  // 0 (16)
    {{0x00000009, 0x00000000, 0x00000000, 0x80000000}},  // -9 (17)
    {{0x00000014, 0x00000000, 0x00000000, 0x00000000}},  // 20 (18)
    {{0x00c3a53d, 0x00000000, 0x00000000, 0x80000000}},  // -12821821 (19)
    {{0x00003215, 0x00000000, 0x00000000, 0x00000000}},  // 12821 (20)
    {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},  // 0 (21)
    {{0x72855207, 0x00000000, 0x00000000, 0x00000000}},  // 1921339911 (22)
    {{0xfffffffd, 0xffffffff, 0xffffffff,
      0x00000000}},  // 79228162514264337593543950333 (23)
    {{0xfffffffe, 0xffffffff, 0xffffffff,
      0x00000000}},  // 79228162514264337593543950334  (24)
    {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},  // 0 (25)
    {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},  // 0 (26)
    {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},  // 0 (27)
    {{0x00000000, 0x00000000, 0x00000000, 0x80000000}},  // -0 (28)
    {{0x00000000, 0x00000000, 0x00000000, 0x80000000}},  // -0 (29)
    {{0x00000000, 0x00000000, 0x00000000, 0x80000000}},  // -0 (30)
};

START_TEST(s21_decimal_truncate_test) {
  for (size_t i = 0; i < sizeof(num1) / sizeof(s21_decimal); i++) {
    s21_decimal tmp = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}};
    int ret = s21_truncate(num1[i], &tmp);

    ck_assert_int_eq(tmp.bits[0], result_truncate[i].bits[0]);
    ck_assert_int_eq(tmp.bits[1], result_truncate[i].bits[1]);
    ck_assert_int_eq(tmp.bits[2], result_truncate[i].bits[2]);
    ck_assert_int_eq(tmp.bits[3], result_truncate[i].bits[3]);

    ck_assert_int_eq(ret, 0);
  }
}
END_TEST


Suite *s_s21_decimal_truncate() {
  Suite *s;
  TCase *tc_truncate;
  s = suite_create("truncate_suit");
  tc_truncate = tcase_create("tc_truncate");
  tcase_add_test(tc_truncate, s21_decimal_truncate_test);
  suite_add_tcase(s, tc_truncate);
  return s;
}