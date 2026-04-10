CC      = gcc
CFLAGS  = -Wall -g -Wpedantic -Wextra -Werror -Wunused -Iinclude

SRC     = $(wildcard src/*.c)
OBJ     = $(patsubst src/%.c, build/%.o, $(SRC))
TARGET  = main 

# vaddr.c lives in the workspace root (not src/) because it is the
# student-editable file that gets submitted for grading.
$(TARGET): $(OBJ) build/vaddr.o
	$(CC) $(CFLAGS) -o $@ $^

build/vaddr.o: vaddr.c
	mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

build/%.o: src/%.c
	mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build $(TARGET)

