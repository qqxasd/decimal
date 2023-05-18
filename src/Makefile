SOURCES=s21_*.c
OBJECTS=s21_*.o
LFLAGS = -lm -lpthread -lcheck
LEAK = -fsanitize=leak -fsanitize=address -fsanitize=undefined -fsanitize=unreachable -fno-sanitize-recover -fstack-protector -fanalyzer

ifeq ($(shell uname -s), Linux)
	LFLAGS = -lm -lpthread -lcheck -lsubunit
endif

all: clean s21_decimal.a test

s21_decimal.a:
	gcc $(FLAGS) -c $(SOURCES)
	ar rc s21_decimal.a $(OBJECTS)
	ranlib s21_decimal.a


test: clean s21_decimal.a
	gcc -c test.c
	gcc test.o $(FLAGS) $(LFLAGS) s21_decimal.a -o test
	rm -rf *.o


gcov_report: clean add_coverage test
	./test
	lcov -t "test" -o test.info -c -d .
	genhtml -o report test.info
	open report/index.html
	
clean:
	rm -rf *.o gcov_report *.gcda *.gcno test test.info

add_coverage:
	$(eval FLAGS += --coverage)
