#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "malloc.h"

#define NUM_ALLOCS 100

int main() {
  void *ptrs[NUM_ALLOCS];

  for (int i = 0; i < NUM_ALLOCS; i++) {
    ptrs[i] = malloc(i + 1);
    assert(ptrs[i] != NULL);
  }

  // Free memory
  for (int i = 0; i < NUM_ALLOCS; i++) {
    free(ptrs[i]);
  }

  printf("\n\nBasic allocation and free test passed.\n");
  return 0;
}