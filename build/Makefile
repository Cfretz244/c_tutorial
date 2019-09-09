CC = gcc
STD = c99

all: unit

unit: unit_one.o unit_two.o
	$(CC) $^ -o $@

%.o: %.c
	 $(CC) -std=$(STD) -c $< -o $@

clean:
	rm *.o
	rm unit

.PHONY: clean
