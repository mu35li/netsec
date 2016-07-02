.PHONY: default object clean test lib

# declarations #

### compiler ###
# gcc because it sometimes makes faster executables than clang
CC = gcc

### compilation flags ###
# -g: for debugging symbols
# -Wall: show all warnings
# -Wextra: no, really, ALL warnings
# -Wshadow: check for shadowed variables
# -Wpedantic: check for strict compliance with ISO C
CFLAGS = -g -Wall -Wextra -Werror -Wshadow -Wpedantic

### objects ###
# the object files we're building
OBJ = simple_aes.o mitm.o

### executable ###
# the name of our executable
# note that .x is just a convention
EXECUTABLE = mitm.x

# targets #

### default target ###
# compile executable
default: mitm.x

### opti target ###
# compile executable with optimizations,
# overriding the default CFLAGS.
# -O3: optimize for speed
# -march=native: allow CPU-specific optimizations
# -flto: do link-time-optimization
opti: CFLAGS = -O3 -march=native -flto
opti: mitm.x

### executable target ###
# link all the object files together
# to build the executable
mitm.x: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(EXECUTABLE)

### objects target ###
# compile C-files without linking to .o (object) files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

### clean target ###
# remove object and executable files
clean:
	rm -f *.o *.x
