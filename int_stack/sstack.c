/*----- System Includes -----*/

#include <assert.h>

/*----- Project Includes -----*/

#include "sstack.h"

/*----- Function Implementations -----*/

inline static void sanity_check(sstack_t const* stk) {
  // Make sure our basic invariants hold.
  assert(stk && stk->pos < SSTACK_SIZE && stk->pos >= SSTACK_BASE);
}

int sstack_init(sstack_t* stk) {
  if (stk) {
    // Set the position to negative one
    // We can leave the actual stack unitialized,
    // because the pos tells us that the stack is empty.
    stk->pos = SSTACK_BASE;
    errno = 0;
    return 0;
  } else {
    errno = EINVAL;
    return -1;
  }
}

void sstack_destroy(sstack_t* stk) {
  // Currently a no-op as the stack doesn't
  // own any resources.
  (void) stk;
}

int sstack_push(sstack_t* stk, int64_t const* val) {
  // Check error conditions.
  sanity_check(stk);
  int64_t target = stk->pos + 1;
  if (target == SSTACK_SIZE) {
    // Stack is full.
    errno = ENOMEM;
    return -1;
  } else if (!val) {
    // User didn't give us a value to push.
    errno = EINVAL;
    return -1;
  }

  // Write the value into the stack.
  stk->stk[target] = *val;

  // Publish and return.
  ++stk->pos;
  errno = 0;
  return 0;
}

int64_t const* sstack_peek(sstack_t* stk) {
  // Check error conditions.
  sanity_check(stk);
  if (stk->pos < 0) {
    errno = ENOENT;
    return NULL;
  }

  // Return the current value.
  errno = 0;
  return &stk->stk[stk->pos];
}

int sstack_pop(sstack_t* stk) {
  // Check error conditions.
  sanity_check(stk);
  if (stk->pos == SSTACK_BASE) {
    errno = ENOENT;
    return -1;
  }

  // Publish and return.
  --stk->pos;
  errno = 0;
  return 0;
}

size_t sstack_size(sstack_t const* stk) {
  // Check error conditions.
  sanity_check(stk);

  // Return the size.
  return stk->pos + 1;
}

size_t sstack_capacity(sstack_t const* stk) {
  (void) stk;
  return SSTACK_SIZE;
}
