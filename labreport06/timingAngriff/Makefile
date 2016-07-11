.PHONY: default clean

# declarations #

### compiler ###
CC = gcc

### compilation flags ###
#CFLAGS = -g -Wall -Wextra -Werror -Wshadow -Wpedantic
CFLAGS = -O0

### objects ###
OBJ = password_compare.o timing.o

### executable ###
EXECUTABLE = timing.x

# targets #

### default target ###
# compile executable
default: $(EXECUTABLE)

### executable target ###
$(EXECUTABLE): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(EXECUTABLE)

### objects target ###
# compile C-files without linking to .o (object) files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
