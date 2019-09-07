#ifndef GSTACK_H
#define GSTACK_H

/*----- System Includes -----*/

#include <errno.h>
#include <stddef.h>
#include <inttypes.h>

/*----- Numerical Constants -----*/

#define GSTACK_SIZE        (4096)
#define GSTACK_BASE        (-1)

/*----- Type Declarations -----*/

typedef struct generic_stack {
  int64_t pos, max;
  size_t record_size;
  char buffer[GSTACK_SIZE];
} gstack_t;

/*----- Function Declarations -----*/

// Lifecycle functions
int gstack_init(gstack_t* stk, size_t record_size);
void gstack_destroy(gstack_t* stk);

// Stack operations
int gstack_push(gstack_t* stk, void const* val);
void const* gstack_peek(gstack_t* stk);
int gstack_pop(gstack_t* stk);
size_t gstack_size(gstack_t const* stk);
size_t gstack_capacity(gstack_t const* stk);

#endif
