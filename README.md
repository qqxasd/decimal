# Decimal

This is an implementation of the decimal.h library for macos/linux.


# Usage

* Go to the src directory and run `make` to get the functions as a static lib (decimal.a), or just copy decimal.c and decimal.h files into your project.

* Run `make test` to make sure that the functions are working correctly.

# Functions

### Arithmetic Operators

| Operator name | Operators  | Function                                                                           | 
| ------ | ------ |------------------------------------------------------------------------------------|
| Addition | + | int add(decimal value_1, decimal value_2, decimal *result)         |
| Subtraction | - | int sub(decimal value_1, decimal value_2, decimal *result) |
| Multiplication | * | int mul(decimal value_1, decimal value_2, decimal *result) | 
| Division | / | int div(decimal value_1, decimal value_2, decimal *result) |

The functions return the error code:
- 0 - OK
- 1 - the number is too large or equal to infinity
- 2 - the number is too small or equal to negative infinity
- 3 - division by 0

*Note on the numbers that do not fit into the mantissa:*
- *When getting numbers that do not fit into the mantissa during arithmetic operations, use bank rounding (for example, 79,228,162,514,264,337,593,543,950,335 - 0.6 = 79,228,162,514,264,337,593,543,950,334)*

### Comparison Operators

| Operator name | Operators  | Function | 
| ------ | ------ | ------ |
| Less than | < | int is_less(decimal, decimal) |
| Less than or equal to | <= | int is_less_or_equal(decimal, decimal) | 
| Greater than | > |  int is_greater(decimal, decimal) |
| Greater than or equal to | >= | int is_greater_or_equal(decimal, decimal) | 
| Equal to | == |  int is_equal(decimal, decimal) |
| Not equal to | != |  int is_not_equal(decimal, decimal) |

Return value:
- 0 - FALSE
- 1 - TRUE

### Convertors and parsers

| Convertor/parser | Function | 
| ------ | ------ |
| From int  | int from_int_to_decimal(int src, decimal *dst) |
| From float  | int from_float_to_decimal(float src, decimal *dst) |
| To int  | int from_decimal_to_int(decimal src, int *dst) |
| To float  | int from_decimal_to_float(decimal src, float *dst) |

Return value - code error:
- 0 - OK
- 1 - convertation error

*Note on the conversion of a float type number:*
- *If the numbers are too small (0 < |x| < 1e-28), return an error and value equal to 0*
- *If the numbers are too large (|x| > 79,228,162,514,264,337,593,543,950,335) or are equal to infinity, return an error*
- *When processing a number with the float type, convert all the significant decimal digits contained in it. If there are more than 7 such digits, the number is rounded to the closest one that does not have more than 7 significant decimal digits.*

*Note on the conversion from decimal type to int:*
- *If there is a fractional part in a decimal number, it should be discarded (for example, 0.9 is converted to 0)*


### Another functions

| Description | Function                                                         | 
| ------ |------------------------------------------------------------------|
| Rounds a specified Decimal number to the closest integer toward negative infinity. | int floor(decimal value, decimal *result)            |	
| Rounds a decimal value to the nearest integer. | int round(decimal value, decimal *result)    |
| Returns the integral digits of the specified Decimal; any fractional digits are discarded, including trailing zeroes. | int truncate(decimal value, decimal *result) |
| Returns the result of multiplying the specified Decimal value by negative one. | int negate(decimal value, decimal *result)   |

Return value - code error:
- 0 - OK
- 1 - calculation error

# Dependencies

project dependencies:
* gcc 10.3.0

test dependencies:
* lcov 2.0-1
* lib check 0.15.2