.PHONY: default object clean cleanup test lib
CC = gcc
CFLAGS = -g -Wall -Wextra -Werror -Wshadow -Wstrict-overflow -Wpedantic
OBJ = simple_aes.o mitm.o
EXECUTABLE = mitm.x

default: mitm.x

opti: CFLAGS = -O3 -march=native
opti: mitm.x

mitm.x: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(EXECUTABLE)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o *.x

cleanup: clean
	rm -f *.so
