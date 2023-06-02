import decimal
import random
import math

import subprocess


def convert_py_decimal_to_4_ints(decimal_num: decimal.Decimal) -> tuple[int, int, int, int]:
    decimal_tuple = decimal_num.as_tuple()
    decimal_int = 0
    digits_exp = 0
    for digit in reversed(decimal_tuple.digits):
        decimal_int += digit * (10 ** digits_exp)
        digits_exp += 1

    third_int = abs(abs(decimal_tuple.sign << 31) + abs(decimal_tuple.exponent << 16))
    zer_int = decimal_int % (2**32)
    decimal_int //= 2 ** 32
    first_int = decimal_int % (2 ** 32)
    decimal_int //= 2 ** 32
    return zer_int, first_int, decimal_int, third_int


def convert_4_ints_to_py_decimal(c_decimal: tuple[int, int, int, int]) -> decimal.Decimal:
    sign = c_decimal[3] >> 31
    exp = ((c_decimal[3]) >> 16) % 32
    decimal_int = int(2 ** 64) * c_decimal[2] + int(2 ** 32) * c_decimal[1] + c_decimal[0]
    decimal_digits = []
    while decimal_int:
        decimal_digits.append(decimal_int % 10)
        decimal_int //= 10
    return decimal.Decimal((sign, tuple(reversed(decimal_digits)), -exp))


def generate_random_decimal() -> decimal.Decimal:
    new_exp = random.randint(-28, 0)
    new_sign = random.randint(0, 1)
    decimal_int = random.randint(0, int(2**96) - 1)
    decimal_digits = []
    while decimal_int:
        decimal_digits.append(decimal_int % 10)
        decimal_int //= 10
    return decimal.Decimal((new_sign, tuple(reversed(decimal_digits)), new_exp))


# decimal.getcontext().Emax = 28
# decimal.getcontext().Emin = 0

# decimal_number1 = decimal.Decimal('792281625142643375935.43950335')
# print('0x%08X 0x%08X 0x%08X 0x%08X' % convert_py_decimal_to_4_ints(decimal_number1))

# print(convert_4_ints_to_py_decimal((0x77D5E3AA, 0x0000011C, 0x00000000, 0x00060000)))
# print(convert_4_ints_to_py_decimal((0x12a495, 0x00000000, 0x00000000, 0x60000)))
# x = 396140812663555.40835774234624
# print(x)
# str_int = x.__str__()
# dec = decimal.Decimal(str_int)
# print('0x%08X 0x%08X 0x%08X 0x%08X' % convert_py_decimal_to_4_ints(dec))
# str_int = round(x).__str__()
# dec = decimal.Decimal(str_int)
# print('0x%08X 0x%08X 0x%08X 0x%08X' % convert_py_decimal_to_4_ints(dec))



decimalus = [(0x00000000, 0x00000000, 0x00000000, 0x80000000),
(0x00000001, 0x00000000, 0x00000000, 0x000e0000),
(0x00000019, 0x00000000, 0x00000000, 0x80010000),
(0x00000183, 0x00000000, 0x00000000, 0x80020000),
(0x14490831, 0x00048e23, 0x00000000, 0x80070000),
(0x00000001, 0x00000000, 0x00000000, 0x80000000),
(0x77d5e3aa, 0x0000011c, 0x00000000, 0x80060000),
(0x80000000, 0x80000000, 0x80000000, 0x001c0000),
(0x80000000, 0x80000000, 0x80000000, 0x800e0000),
(0x00000001, 0x00000000, 0x00000000, 0x801c0000),
(0x000f4240, 0x00000000, 0x00000000, 0x801c0000),
(0x80000000, 0x80000000, 0x80000000, 0x80000000),
(0xffffffff, 0xffffffff, 0xffffffff, 0x80010000),
(0xffffffff, 0xffffffff, 0xffffffff, 0x80000000),
(0x1422cf6c, 0x000001fc, 0x00000000, 0x00030000),
(0x2564b1ea, 0x3927f1f3, 0x041c50d7, 0x800b0000),
(0x0001869f, 0x00000000, 0x00000000, 0x80050000),
(0x0098967e, 0x00000000, 0x00000000, 0x00060000),
(0x000000cf, 0x00000000, 0x00000000, 0x80010000),
(0x0707e3c5, 0x002d8d61, 0x00000000, 0x00090000),
(0x846a4b4c, 0x0000012a, 0x00000000, 0x80080000),
(0x000035a3, 0x00000000, 0x00000000, 0x80170000),
(0x79353447, 0x00000004, 0x00000000, 0x80010000),
(0xfffffffd, 0xffffffff, 0xffffffff, 0x80000000),
(0xfffffffe, 0xffffffff, 0xffffffff, 0x80000000),
(0x00000005, 0x00000000, 0x00000000, 0x80010000),
(0x00000033, 0x00000000, 0x00000000, 0x80020000),
(0x00000031, 0x00000000, 0x00000000, 0x80020000),
(0x00000005, 0x00000000, 0x00000000, 0x00010000),
(0x00000033, 0x00000000, 0x00000000, 0x00020000),
(0x00000031, 0x00000000, 0x00000000, 0x00020000),]
j = 0
for i in decimalus:
    # print(f"{j} = ",end = '')
    # j = j+1
    print("// ", end = '')
    print(convert_4_ints_to_py_decimal((i)), end = ' ')
    print(f"({j})")
    j = j+1
# print(convert_4_ints_to_py_decimal((0x14490831, 0x48e23, 0x0, 0x70000)))
# print(convert_4_ints_to_py_decimal((0x22, 0x0, 0x0, 0x0)))




# new_decimal = generate_random_decimal()
# print(new_decimal)
# print('0x%08X 0x%08X 0x%08X 0x%08X' % convert_py_decimal_to_4_ints(new_decimal))
#687194767 4123168604 42949672
#792281625142643375935439
#792281625142643375935440

# -3.9614081266355540835774234624
# -3.961408126635554