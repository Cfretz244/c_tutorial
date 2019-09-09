#ifndef DSTACK_H
#define DSTACK_H

/*----- System Includes -----*/

#include <errno.h>
#include <stddef.h>
#include <inttypes.h>

/*----- Numerical Constants -----*/

#define DSTACK_SIZE        (4096)
#define DSTACK_BASE        (-1)

/*----- Type Declarations -----*/

typedef struct dynamic_stack {
  int64_t pos, capacity;
  size_t record_size;
  void* buffer;
  void (*destroy) (void*);
} dstack_t;

/*----- Function Declarations -----*/

// Lifecycle functions
int dstack_init(dstack_t* stk, size_t record_size, void (*destroy) (void*));
void dstack_destroy(dstack_t* stk);

// Stack operations
int dstack_push(dstack_t* stk, void const* val);
void* dstack_peek(dstack_t* stk);
int dstack_pop(dstack_t* stk);
size_t dstack_size(dstack_t const* stk);
size_t dstack_capacity(dstack_t const* stk);

#endif
