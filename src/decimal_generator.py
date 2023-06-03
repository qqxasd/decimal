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



decimalus = [(0x9BD58675, 0x15E87A6F, 0x42243, 0x0),
(0x9BD58676, 0x15E87A6F, 0x42243, 0x0),
(0x32BBC140, 0x8D8AFA58, 0xC911E, 0x0),
(0x32BBC141, 0x8D8AFA58, 0xC911E, 0x0),
(0x89696969, 0xD80A02F7, 0x284B6DC6, 0x0),
(0x89696969, 0xD80A02F7, 0x284B6DC6, 0x0),
(0xADCFD4F0, 0xD696F916, 0xD4BB, 0x0),
(0xADCFD4F0, 0xD696F916, 0xD4BB, 0x0),
(0x9C29AC02, 0x8B157841, 0x227FB71, 0x0),
(0x9C29AC03, 0x8B157841, 0x227FB71, 0x0),
(0xEF1E9F89, 0xA0889681, 0x1545EC8B, 0x0),
(0xEF1E9F89, 0xA0889681, 0x1545EC8B, 0x0),
(0x15033E4F, 0x3A695850, 0x12FAE9C2, 0x0),
(0xD2206F19, 0x481D7320, 0xBDCD2196, 0x0),
(0xEFB70B12, 0x38053610, 0x2091E9DA, 0x0),
(0xCA87D673, 0x80D52F35, 0x932D6BD1, 0x0),]
j = 0
for i in decimalus:
    if j == 0:
        print(f"DELIMOE = ",end = '')
    if j == 1:
        print(f"DELITEL = ",end = '')
    if j == 3:
        print(f"OSTATOK = ",end = '')
    if j == 2:
        print(f"CHASTNOE = ",end = '')
    if j == 4:
        print(f"TMP RESULT = ",end = '')
    j = (j+1) % 5
    # print("// ", end = '')
    print(convert_4_ints_to_py_decimal((i)))
    # print(f"({j})")
    # j = j+1
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
