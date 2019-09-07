/*----- System Includes -----*/

#include <stdlib.h>
#include <assert.h>
#include <string.h>

/*----- Project Includes -----*/

#include "dstack.h"

/*----- Numerical Constants -----*/

#define STR_LEN           (8)
#define MAX_STRING        (16)
#define NUM_STRINGS       (16)

/*----- Type Declarations -----*/

typedef struct string {
  char* str;
  size_t len;
} string_t;

/*----- Function Implementations -----*/

char* rand_string(size_t len) {
  // Our character set to pull from
  static char const* alpha = "abcdefghijklmnopqrstuvwxyz";

  // Get memory for our string.
  char* str = (char*) malloc(sizeof(char) * (len + 1));

  // Write in our random characters.
  // XXX: The function rand is a TERRIBLE random number generator
  // Don't write this code in production
  char* curr = str;
  while (curr - str < len) {
    *curr++ = alpha[rand() % 26];
  }

  // Terminate our string and return.
  str[len] = '\0';
  return str;
}

void destroy_string(void* ptr) {
  string_t* str = (string_t*) ptr;
  free(str->str);
}

int main() {
  // Initialize a stack.
  dstack_t stk;
  dstack_init(&stk, sizeof(string_t), destroy_string);

  // Push a bunch of strings.
  string_t curr;
  curr.len = STR_LEN;
  string_t strs[NUM_STRINGS];
  for (int i = 0; i < NUM_STRINGS; i++) {
    // Allocate a random string.
    curr.str = rand_string(STR_LEN);

    // Copy the string into the stack,
    // and remember it.
    strs[i] = curr;
    dstack_push(&stk, &curr);
  }

  // Pop things off the stack and ensure things
  // come out in the right order.
  int idx = NUM_STRINGS - 1;
  while (dstack_size(&stk)) {
    // Check to ensure things come back out of the stack
    // in the correct order, and that no values are missed.
    string_t const* curr = (string_t const*) dstack_peek(&stk);
    assert(!memcmp(curr, &strs[idx--], sizeof(string_t)));

    // Update and continue.
    int err = dstack_pop(&stk);
    assert(!err);
  }
  assert(idx == -1);
  assert(!dstack_peek(&stk));
  assert(errno == ENOENT);

  // Cleanup and exit.
  dstack_destroy(&stk);
  return 0;
}
