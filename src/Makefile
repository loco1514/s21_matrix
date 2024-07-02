CC = gcc
OS = $(shell uname)
ifeq ($(OS), Linux)
 LIBS = -lcheck -lpthread -lrt -lsubunit -lm
 LCOV = -o
else
 LIBS = -lcheck -lpthread -lm
 LCOV = --ignore-errors unmapped -o
endif

CFLAGS = -c -Wall -Werror -Wextra -std=c11 $(LIBS)

all: s21_matrix.a

test: gcov_s21_matrix.a
	$(CC) $(CFLAGS) test.c
	$(CC) --coverage test.c s21_matrix.a $(LIBS) -o dec_test
	./dec_test

clean:
	rm -rf *.o *.info s21_*_test *.gcda *.gcno *.css *.html *.gcov *.a report dec_test

gcov_report: test
	lcov -t "dec_test" -o gcov.test.info -c -d .
	genhtml $(LCOV) report gcov.test.info
	open ./report/index.html

gcov_s21_matrix.a:
	rm -f s21_matrix.a
	$(CC) --coverage $(CFLAGS) s21_*.c
	ar rc s21_matrix.a s21_*.o
	ranlib s21_matrix.a

s21_matrix.a: s21_*.c
	$(CC) $(CFLAGS) s21_*.c
	ar rc s21_matrix.a s21_*.o
	ranlib s21_matrix.a

rebuild: clean all

style:
	cp ../materials/linters/.clang-format .
	clang-format -n *.c *.h
	rm -rf .clang-format

style_fix:
	cp ../materials/linters/.clang-format .
	clang-format -i *.c *.h
	rm -rf .clang-format

check_valgrind: 
	valgrind --tool=memcheck --leak-check=yes ./dec_test

check_leaks: 
	leaks -atExit -- ./dec_test
