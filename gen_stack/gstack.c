/*----- System Includes -----*/

#include <assert.h>
#include <string.h>

/*----- Project Includes -----*/

#include "gstack.h"

/*----- Function Implementations -----*/

inline static void sanity_check(gstack_t const* stk) {
  // Make sure our basic invariants hold.
  assert(stk && stk->pos < GSTACK_SIZE && stk->pos >= GSTACK_BASE && stk->max);
}

inline static void* calc_ptr(gstack_t* stk, int64_t pos) {
  return stk->buffer + (pos * stk->record_size);
}

int gstack_init(gstack_t* stk, size_t record_size) {
  // Calculate how many records our stack can hold
  // Integer division will round down and give us a
  // conservative estimate.
  int64_t max = GSTACK_SIZE / record_size;
  if (!max) {
    // Requested record size is too large, we can't
    // even fit a single record in the stack.
    errno = ENOSPC;
    return -1;
  }

  // If we were given something, initialize it.
  if (stk) {
    stk->max = max;
    stk->pos = GSTACK_BASE;
    stk->record_size = record_size;
    errno = 0;
    return 0;
  } else {
    errno = EINVAL;
    return -1;
  }
}

void gstack_destroy(gstack_t* stk) {
  // Currently a no-op as the stack doesn't
  // own any resources.
  (void) stk;
}

int gstack_push(gstack_t* stk, void const* val) {
  // Check error conditions.
  sanity_check(stk);
  int64_t target = stk->pos + 1;
  if (target == stk->max) {
    // Stack is full.
    errno = ENOMEM;
    return -1;
  } else if (!val) {
    // User didn't give us a value to push.
    errno = EINVAL;
    return -1;
  }

  // Write the value into the stack.
  // We cannot perform a simple dereference here,
  // unlike the simple stack example, because the
  // compiler can't statically know how many bytes
  // to copy, since this is now a generic stack.
  // memcpy is the general purpose way to copy memory
  // when the size of the region to be copied can't be
  // known until runtime.
  // We calculate the address of the current record,
  // then call memcpy to copy in whatever value we
  // were given.
  memcpy(calc_ptr(stk, target), val, stk->record_size);

  // Publish and return.
  ++stk->pos;
  errno = 0;
  return 0;
}

void const* gstack_peek(gstack_t* stk) {
  // Check error conditions.
  sanity_check(stk);
  if (stk->pos < 0) {
    errno = ENOENT;
    return NULL;
  }

  // Return the current value.
  errno = 0;
  return calc_ptr(stk, stk->pos);
}

int gstack_pop(gstack_t* stk) {
  // Check error conditions.
  sanity_check(stk);
  if (stk->pos == GSTACK_BASE) {
    errno = ENOENT;
    return -1;
  }

  // Publish and return.
  --stk->pos;
  errno = 0;
  return 0;
}

size_t gstack_size(gstack_t const* stk) {
  // Check error conditions.
  sanity_check(stk);

  // Return the size.
  return stk->pos + 1;
}

size_t gstack_capacity(gstack_t const* stk) {
  return stk->max;
}
