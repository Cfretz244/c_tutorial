/*----- System Header Files -----*/

#include <stdio.h>
#include <errno.h>

/*----- Numerical Constants -----*/

#define HEAP_SIZE           4096

/*----- Function Implementations -----*/

static void* malloc(size_t len) {
  // Static buffers will stay in scope for the full
  // life of the program.
  static char buffer[HEAP_SIZE];
  static char* curr = buffer;

  // Check if we have any space left.
  if ((curr + len) - buffer < HEAP_SIZE) {
    // Return the current value of the allocation pointer
    // and then increment it so that the next allocation
    // will grab a different bit of memory.
    void* section = curr;
    curr += len;
    return section;
  } else {
    // Out of space, return failure and set errno accordingly.
    errno = ENOMEM;
    return NULL;
  }
}

static void free(void* ptr) {
  (void) ptr;
}

static size_t strlen(char const* str) {
  // Make a copy of our pointer, walk it forward
  // until we hit the null terminator, then use
  // pointer arithmetic to work out how far we walked.
  char const* curr = str;
  while (*curr) curr++;
  return curr - str;
}

static char* strdup(char const* str) {
  // Find out how large the string is
  // and allocate enough memory to copy it.
  size_t len = strlen(str) + 1;
  char* buff = malloc(sizeof(char) * len);
  if (!buff) return NULL;

  // Walk both pointers forward and copy as we go.
  while (*str) *buff++ = *str++;

  // Null terminate and return.
  buff[len] = '\0';
  return buff;
}

char* do_reverse(char const* in, char* out) {
  // Check if our incoming pointer has hit
  // the null terminator yet.
  if (*in) {
    // Not at the end of the string yet, recurse.
    out = do_reverse(in + 1, out);
    *out = *in;
    return out + 1;
  } else {
    // Base case, in points at the null terminator,
    // which means we've reached the end of the string.
    return out;
  }
}

char* reverse(char const* str) {
  // Create a dynamically allocated copy of this
  // null terminated string.
  char* reversed = strdup(str);
  if (!reversed) return NULL;

  // Reverse it and return.
  do_reverse(str, reversed);
  return reversed;
}

int main() {
  char* reversed = reverse("!dlrow olleH");
  printf("Reversed string: \"%s\"\n", reversed);
  free(reversed);
  return 0;
}
