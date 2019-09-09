/*----- System Includes -----*/

#include <stdlib.h>
#include <assert.h>
#include <string.h>

/*----- Project Includes -----*/

#include "gstack.h"

/*----- Numerical Constants -----*/

#define STR_LEN           (8)
#define MAX_STRING        (16)
#define NUM_STRINGS       (16)

/*----- Type Declarations -----*/

typedef struct string {
  size_t len;
  char str[MAX_STRING];
} string_t;

/*----- Function Implementations -----*/

char* rand_string() {
  // Our character set to pull from
  static char const* alpha = "abcdefghijklmnopqrstuvwxyz";

  // Get memory for our string.
  char* str = (char*) malloc(sizeof(char) * (STR_LEN + 1));

  // Write in our random characters.
  // XXX: The function rand is a TERRIBLE random number generator
  // Don't write this code in production
  char* curr = str;
  while (curr - str < STR_LEN) {
    *curr++ = alpha[rand() % 26];
  }

  // Terminate our string and return.
  str[STR_LEN] = '\0';
  return str;
}

string_t* alloc_string(char const* str) {
  // Get the length of the string.
  size_t len = strlen(str);

  // Check if it's too large.
  if (len >= MAX_STRING) {
    errno = EINVAL;
    return NULL;
  }

  // Allocate and copy
  // strcpy automatically copies the null-terminator,
  // so we don't need to explicitly terminate the string.
  string_t* dup = (string_t*) malloc(sizeof(string_t));
  strcpy(dup->str, str);
  dup->len = len;
  return dup;
}

string_t** gen_strings() {
  // Allocate space for our array.
  string_t** strs = (string_t**) malloc(sizeof(string_t*) * NUM_STRINGS);

  // Generate all of the random strings.
  for (int i = 0; i < NUM_STRINGS; ++i) {
    // Allocate a random string.
    char* str = rand_string();

    // Convert it into our custom string type.
    strs[i] = alloc_string(str);

    // alloc_string does not take ownership of str,
    // so we are still responsible for freeing it.
    free(str);
  }
  return strs;
}

int main() {
  // Initialize a stack.
  gstack_t stk;
  gstack_init(&stk, sizeof(string_t));

  // Generate a bunch of random strings.
  string_t** strs = gen_strings();

  // Push all of our strings.
  for (int i = 0; i < NUM_STRINGS; i++) {
    int err = gstack_push(&stk, strs[i]);
    assert(!err);
  }

  // Pop things off the stack and ensure things
  // come out in the right order.
  int idx = NUM_STRINGS - 1;
  while (gstack_size(&stk)) {
    // Current value on the stack should be
    // one less than our local value.
    string_t* curr = (string_t*) gstack_peek(&stk);
    assert(!strcmp(curr->str, strs[idx--]->str));

    // Update and continue.
    int err = gstack_pop(&stk);
    assert(!err);
  }
  assert(idx == -1);
  assert(!gstack_peek(&stk));
  assert(errno == ENOENT);

  // Cleanup and exit.
  gstack_destroy(&stk);
  return 0;
}
