import decimal
import random


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

print(convert_4_ints_to_py_decimal((3600900580, 1161359156, 4294537799, 0x00040000)))
print(convert_4_ints_to_py_decimal((0xffffffff, 0xffffffff, 0xffffffff, 0x00040000)))

# new_decimal = generate_random_decimal()
# print(new_decimal)
# print('0x%08X 0x%08X 0x%08X 0x%08X' % convert_py_decimal_to_4_ints(new_decimal))
#687194767 4123168604 42949672
#792281625142643375935439
#792281625142643375935440
