#include <stdio.h>

void print_arguments(char** argv) {
  while (*argv) printf("Argument: \"%s\"\n", *argv++);
}
