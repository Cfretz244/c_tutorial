/*----- System Includes -----*/

#include <assert.h>

/*----- Project Includes -----*/

#include "sstack.h"

/*----- Function Implementations -----*/

int main() {
  // Initialize a stack.
  sstack_t stk;
  sstack_init(&stk);

  // Push until the stack fills.
  int err;
  int64_t val = 0;
  do {
    err = sstack_push(&stk, &val);
  } while (!err && ++val);
  assert(errno == ENOMEM);

  // Pop things off the stack and ensure things
  // come out in the right order.
  while (sstack_size(&stk)) {
    // Current value on the stack should be
    // one less than our local value.
    int64_t* curr = sstack_peek(&stk);
    assert(*curr + 1 == val);

    // Update and continue.
    err = sstack_pop(&stk);
    assert(!err);
    --val;
  }
  assert(val == 0);
  assert(!sstack_peek(&stk));
  assert(errno == ENOENT);

  // Cleanup and exit.
  sstack_destroy(&stk);
  return 0;
}
