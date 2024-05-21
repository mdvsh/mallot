//
// Created by Madhav Shekhar Sharma on 5/21/24.
//

#ifndef MALLOT_SRC_MALLOC_H_
#define MALLOT_SRC_MALLOC_H_

#include <pthread.h>
#include <sys/mman.h>
#include <stddef.h>

#define NUM_LISTS 128
#define ALIGNMENT 8
#define PAGE_SIZE 4096
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~(ALIGNMENT-1))
#define BLOCK_SIZE sizeof(block)

typedef struct block {
  size_t size;
  int free;
  char data[1];
  struct block* next;
} m_block;

// ---------------- utils ----------------
static size_t get_seg_free_list_idx(size_t size);
static void *mmap_helper(size_t size);

// ---------------- core ----------------
void *malloc(size_t size);
static void *expand_memory(size_t size);
static void *mmap_helper(size_t size);
void free(void *ptr);

#endif // MALLOT_SRC_MALLOC_H_
