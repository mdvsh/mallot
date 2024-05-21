#include "malloc.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void test_calloc() {
  int *arr = (int *)calloc(10, sizeof(int));
  assert(arr != NULL);

  for (int i = 0; i < 10; i++) {
    assert(arr[i] == 0);
  }

  free(arr);
}

void test_realloc() {
  int *arr = (int *)malloc(10 * sizeof(int));
  assert(arr != NULL);

  for (int i = 0; i < 10; i++) {
    arr[i] = i;
  }

  arr = (int *)realloc(arr, 20 * sizeof(int));
  assert(arr != NULL);

  for (int i = 0; i < 10; i++) {
    assert(arr[i] == i);
  }

  free(arr);
}

int main() {
  test_calloc();
  printf("\nBasic calloc test passed.\n");
  test_realloc();
  printf("\nBasic realloc test passed.\n");
}