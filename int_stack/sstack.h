#ifndef SSTACK_H
#define SSTACK_H

/*----- System Includes -----*/

#include <errno.h>
#include <stddef.h>
#include <inttypes.h>

/*----- Numerical Constants -----*/

#define SSTACK_SIZE        (16)
#define SSTACK_BASE        (-1)

/*----- Type Declarations -----*/

typedef struct simple_stack {
  int64_t pos;
  int64_t stk[SSTACK_SIZE];
} sstack_t;

/*----- Function Declarations -----*/

// Lifecycle functions
int sstack_init(sstack_t* stk);
void sstack_destroy(sstack_t* stk);

// Stack operations
int sstack_push(sstack_t* stk, int64_t const* val);
int64_t const* sstack_peek(sstack_t* stk);
int sstack_pop(sstack_t* stk);
size_t sstack_size(sstack_t const* stk);
size_t sstack_capacity(sstack_t const* stk);

#endif
