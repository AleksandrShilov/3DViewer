CC=gcc
CFLAGS=-c -std=c11 -Wall -Werror -Wextra
CFLAGS2=-fprofile-arcs -ftest-coverage
WASH=rm -rf *.o *.a *.out *.log *.aux *.dvi *.toc *css *gcno *gcda CPPLINT.cfg *tgz *.html man_ru .clang-format
OS=$(shell uname)

ifeq ($(OS), Linux)
	LFLAGS = -lcheck -lm -pthread -lgcov --coverage -lrt -lsubunit
	LEAKS=CK_FORK=no valgrind --leak-check=full --track-origins=yes -s ./test.out
else
	LFLAGS = -lcheck -pthread -lgcov --coverage
	LEAKS=CK_FORK=no leaks --atExit -- ./test.out
endif

all: clean 3dviewer.a tests gcov_report

install:
	mkdir build
	cp 3DViewer_1_OpenGL_v_2.pro ./build
	cp -r 3rdparty ./build
	cp *.c *.h *.cpp *.ui ./build
	cd build; qmake 3DViewer_1_OpenGL_v_2.pro
	make -C build
	cd build; rm -rf *.c *.h *.cpp *.ui *.o
	mkdir $(HOME)/Desktop/3DViewer/
	cp -rf build/3DViewer_1_OpenGL_v_2.app $(HOME)/Desktop/3DViewer/
	make clean

uninstall:
	rm -rf $(HOME)/Desktop/3DViewer/
	rm -rf build

dvi:
	makeinfo --html --no-warn --no-validate --force ./man_ru.texi

dist:
	rm -rf My3DViewer
	mkdir My3DViewer
	cp Makefile *.c *.h *.pro *.texi *.cpp *.ui *.user My3DViewer/
	tar -zcvf ./My3DViewer.tar.tgz ./My3DViewer
	rm -rf My3DViewer/

3dviewer.a: parse.o
	ar rc 3dviewer.a parse.o
	ranlib 3dviewer.a

main: main.o parse.o
	$(CC) $^ -lm -o main.out && ./main.out

main.o: main.c
	$(CC) $(CFLAGS) $^

parse.o: parse.c
	$(CC) $(CFLAGS) -c $^

gcov_report: tests
	gcovr -r . --html --html-details -o gcov_report.html
	open gcov_report.html

tests: parse_test.o test.o
	$(CC) $^ -o test.out $(LFLAGS) && ./test.out


parse_test.o: parse.c
	$(CC) $(CFLAGS) $(CFLAGS2) -c $^ -o $@

tests.o: test.c
	$(CC) $(CFLAGS2) -c $^ -o $@

check_clang:
	cp ../materials/linters/.clang-format .clang-format
	# clang-format -i parse.c parse.h
	clang-format -n *.c *.h


check_leaks:
	$(LEAKS)

check_cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem main.c parse.c test.c

clean:
	$(WASH)