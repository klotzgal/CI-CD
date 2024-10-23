GC=gcc
CFLAGS=-Wall -Wextra -Werror
OB=s21_grep
LIB=grep_lib

all: $(OB)

$(OB): $(OB).o $(LIB).a
	$(GC) $(CFLAGS) $(OB).o -L. $(LIB).a -o $(OB)
	make clean

$(OB).o : $(OB).c
	$(GC) $(CFLAGS) -c $(OB).c -o $(OB).o

$(LIB).o : $(LIB).c
	$(GC) $(CFLAGS) -c $(LIB).c 

$(LIB).a : $(LIB).o
	ar rc $(LIB).a $(LIB).o 
	ranlib $(LIB).a

clean:
	rm -rf *.o
	rm -rf *.a

rebuild:  all

test : $(OB)
	bash test.sh
	rm -rf *.log

leak: $(OB)
	leaks --atExit -- ./$(OB) -e int $(OB).c -ivclnhso test_file/all_flag_tests/*

check: $(OB)
	cp ../../../materials/linters/.clang-format ./
	clang-format -n *.c *.h
	rm -f .clang-format

valgrind:
	sh ../../materials/build/run.sh
