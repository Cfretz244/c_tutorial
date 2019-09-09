/*----- System Includes -----*/

#include <assert.h>
#include <stdlib.h>
#include <string.h>

/*----- Project Includes -----*/

#include "dstack.h"

/*----- Numerical Constants -----*/

#define DSTACK_INIT_CAPACITY              (8)

/*----- Function Implementations -----*/

inline static void sanity_check(dstack_t const* stk) {
  // Make sure our basic invariants hold.
  assert(stk && stk->pos < DSTACK_SIZE && stk->pos >= DSTACK_BASE && stk->capacity);
}

inline static void* calc_ptr(dstack_t* stk, int64_t pos) {
  // Calculate the base address of the requested record.
  // We cast the buffer to a character pointer so that
  // our pointer arithmetic will work in terms of bytes
  // Then we take the requested array position, multiplied
  // by the size of each record to compute the address
  return ((char*) stk->buffer) + (pos * stk->record_size);
}

inline static int extend_stack(dstack_t* stk) {
  // Calculate our new intended capacity.
  int64_t target = stk->capacity * 2;

  // Realloc can be used to extend a previous allocation.
  // If the extension fails, the original buffer will be untouched.
  void* tmp = realloc(stk->buffer, target * stk->record_size);
  if (!tmp) return -1;

  // Stuff worked, the old buffer is now dangling, update and return.
  stk->buffer = tmp;
  stk->capacity *= 2;
  return 0;
}

int dstack_init(dstack_t* stk, size_t record_size, void (*destroy) (void*)) {
  // If we were given something, initialize it.
  if (stk) {
    stk->pos = DSTACK_BASE;
    stk->capacity = DSTACK_INIT_CAPACITY;
    stk->record_size = record_size;
    stk->buffer = malloc(record_size * DSTACK_INIT_CAPACITY);
    stk->destroy = destroy;

    // We need to check if malloc failed, as we could otherwise
    // leak a partially initialized stack.
    if (stk->buffer) {
      errno = 0;
      return 0;
    } else {
      return -1;
    }
  } else {
    errno = EINVAL;
    return -1;
  }
}

void dstack_destroy(dstack_t* stk) {
  // Spin and destroy any remaining
  // values on the stack.
  while (dstack_size(stk)) dstack_pop(stk);

  // Destroy the buffer.
  free(stk->buffer);
}

int dstack_push(dstack_t* stk, void const* val) {
  // Check error conditions.
  sanity_check(stk);
  int64_t target = stk->pos + 1;
  if (target == stk->capacity) {
    // We've hit our current capacity
    // Double the storage if we can, or return error
    int err = extend_stack(stk);
    if (err) return -1;
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

void* dstack_peek(dstack_t* stk) {
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

int dstack_pop(dstack_t* stk) {
  // Check error conditions.
  sanity_check(stk);
  if (stk->pos == DSTACK_BASE) {
    errno = ENOENT;
    return -1;
  }

  // If we were given a destructor
  // Destroy the current value on the stack
  if (stk->destroy) stk->destroy(calc_ptr(stk, stk->pos));

  // Publish and return.
  --stk->pos;
  errno = 0;
  return 0;
}

size_t dstack_size(dstack_t const* stk) {
  // Check error conditions.
  sanity_check(stk);

  // Return the size.
  return stk->pos + 1;
}

size_t dstack_capacity(dstack_t const* stk) {
  return stk->capacity;
}
