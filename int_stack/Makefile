CC = clang
STD = c99
BIN = stack_tests

all: $(BIN)

$(BIN): stack_tests.c stack.o
	$(CC) -std=$(STD) -g $^ -o $@

%.o: %.c
	 $(CC) -std=$(STD) -g -c $< -o $@

clean:
	rm *.o
	rm $(BIN)

.PHONY: clean
