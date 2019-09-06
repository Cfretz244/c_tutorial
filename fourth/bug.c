#include <time.h>
#include <stdlib.h>
#include <string.h>

size_t arrlen(void* arr) {
  void** curr = (void**) arr;
  while (*curr) curr++;
  return curr - (void**) arr;
}

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

char** rand_strings(size_t num, size_t len) {
  // Allocate an array of character pointers
  // We allocate one more than we were asked for so that
  // we can null terminate the array.
  // calloc is like malloc, except it zero-initializes
  // the memory it hands back, unlike malloc.
  char** strs = (char**) calloc(num + 1, sizeof(char*));

  // Generate a random string for each.
  // We don't have to touch the last element in the array
  // because calloc already initialized it to NULL.
  for (size_t i = 0; i < num; ++i) {
    strs[i] = rand_string(len);
  }
  return strs;
}

char* dup_str(char const* str) {
  // XXX: There is a bug in this function!
  char* dup = (char*) malloc(sizeof(char) * strlen(str));
  strcpy(dup, str);
  return dup;
}

char** dup_strs(char** strs) {
  // Figure out how long this array is.
  size_t len = arrlen(strs);

  // Duplicate it.
  char** dups = (char**) calloc(len + 1, sizeof(char*));
  for (size_t i = 0; i < len; ++i) {
    dups[i] = dup_str(strs[i]);
  }
  return dups;
}

void destroy_strs(char** strs) {
  char** curr = strs;
  while (*curr) {
    free(*curr++);
  }
  free(strs);
}

int main(int argc, char** argv) {
  // Seed our random number generator
  // so it doesn't always generate the same strings.
  srand(time(NULL));

  // If the user tells us how many strings to use,
  // and what their length should be, use it.
  // atoi is another function that's easy to use
  // but bad. Don't use it in production.
  size_t num_strs = 8, str_len = 8;
  if (argc >= 2) num_strs = atoi(argv[1]);
  if (argc >= 3) str_len = atoi(argv[2]);

  // Create our strings.
  char** strs = rand_strings(num_strs, str_len);

  // Copy them for whatever reason.
  char** dups = dup_strs(strs);

  // Clean up and exit.
  destroy_strs(dups);
  destroy_strs(strs);
  return 0;
}
