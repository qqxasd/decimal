## Makefile by phyliscr [2023]

## Variables Block
CC = gcc
CCFLAGS = -Werror -Wall -Wextra -std=c11
COVERAGE = 
LIBS = -lpthread -lcheck -lm
LEAK = -fsanitize=leak -fsanitize=address -fsanitize=undefined -fsanitize=unreachable -fno-sanitize-recover -fstack-protector -fanalyzer

SOURCES = $(wildcard s21_*.c)
OBJECTS = $(patsubst %.c,%.o,$(SOURCES))

TESTS_DIR = tests/
TESTS_SRC = $(wildcard $(TESTS_DIR)*.c)
TESTS_OBJ = $(patsubst %.c,%.o,$(notdir $(TESTS_SRC)))

TARGET = s21_decimal.a

ifeq ($(shell uname -s), Linux)
	LIBS = -lpthread -lcheck -lsubunit -lm
endif

## Application Block
.PHONY: all clean test gcov_report

all: gcov_report

clean:
	rm -rf *.o gcov_report *.a *.gcda *.gcno test.info

$(TARGET): clean
	@echo $(TESTS_DIR) $(TESTS_SRC)
	$(CC) $(CCFLAGS) -c $(SOURCES)
	ar rc $@ $(OBJECTS)
	ranlib $@

test: $(TARGET)
	$(CC) $(CCFLAGS) $(COVERAGE) -c $(TESTS_SRC)
	$(CC) $(CCFLAGS) $(COVERAGE) $(TESTS_OBJ) $(LIBS) $(TARGET) -o $@
	rm -rf $@.o

gcov_report: add_coverage test
	./test
	lcov -t "test" -o test.info -c -d .
	genhtml -o report test.info
ifeq ($(shell uname -s), Macosx)
	open report/index.html
endif

add_coverage:
	$(eval COVERAGE += --coverage)

## Check Block
.PHONY: check clang cppcheck cpplint leaks leaks_debug gcc_leaks
check: clang cppcheck cpplint leaks

clang:
	@echo ""
	@echo "clang-format: Error."
	@clang-format -style=Google -n *.c
	@echo "clang-format: Correction."
	@clang-format -style=Google -i *.c
	@echo "clang-format: Check."
	@clang-format -style=Google -n *.c

cppcheck:
	@echo -n "\ncppcheck: "
	@cppcheck --enable=all --suppress=missingIncludeSystem .

cpplint:
	cp ../materials/linters/CPPLINT.cfg CPPLINT.cfg
	python3 ../materials/linters/cpplint.py --extension=c *.c *.h

leaks: test
	valgrind --leak-check=full --log-file=RESULT_VALGRIND.txt ./test; cat RESULT_VALGRIND.txt| grep "ERROR SUMMARY:"
	##valgrind --leak-check=full --log-file=RESULT_VALGRIND1.txt ./test1; cat RESULT_VALGRIND1.txt| grep "ERROR SUMMARY:"

leaks_debug: ## If a leak is detected !!!
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
			 --verbose --log-file=RESULT_VALGRIND.txt ./test 

## https://github.com/google/sanitizers/wiki/AddressSanitizerComparisonOfMemoryTools
gcc_leaks: clean
	$(CC) $(CCFLAGS) -c $(C_FILE)
	-ggdb -g -fsanitize=leak -fsanitize=address -fsanitize=undefined \
	-fsanitize=unreachable -fno-sanitize-recover -fstack-protector -fanalyzer \
	
arithmetic_tests: clean
	gcc $(ССFLAGS) -c $(SOURCES)
	ar rc s21_decimal.a $(OBJECTS)
	ranlib s21_decimal.a
	gcc -c test.c $(COVERAGE)
	gcc test.o $(CCFLAGS) $(LIBS) $(COVERAGE) s21_decimal.a -o test
	rm -rf *.o

convert_test: clean
	gcc $(CCFLAGS) -c $(SOURCES)
	ar rc s21_decimal.a $(OBJECTS)
	ranlib s21_decimal.a
	gcc -c tests/convertion_test.c $(COVERAGE)
	gcc convertion_test.o $(CCFLAGS) $(LIBS) $(COVERAGE) s21_decimal.a -o test
	rm -rf *.o
