CC = clang
STD = c99
BIN = stack_tests

all: $(BIN)

$(BIN): stack_tests.c gstack.o
	$(CC) -std=$(STD) $^ -o $@

%.o: %.c
	 $(CC) -std=$(STD) -c $< -o $@

clean:
	rm *.o
	rm $(BIN)

.PHONY: clean
