CC = gcc
CFLAGS = -g -Wall -Wshadow
DEPS = main.c token.h scanner.c scanner.h testScanner.c testScanner.h
OBJ = main.o scanner.o testScanner.o
EXEC = P1
TESTS = P1_test1.txt P1_test2.txt P1_test3.txt
TARDEPS = Makefile README.txt

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

P1: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: tar clean

tar:
	tar -cvf tiffany-nguyen-P1.tar $(DEPS) $(TARDEPS) $(TESTS)

clean:
	rm $(EXEC) $(OBJ)
