//
// Created by Madhav Shekhar Sharma on 5/21/24.
//

#include "malloc.h"

static m_block *seg_free_list[NUM_LISTS];
static pthread_mutex_t seg_free_list_mutex = PTHREAD_MUTEX_INITIALIZER;
static m_block *mem_pool = NULL;
static size_t used_mem = 0;
static size_t mem_pool_size = 0;

static size_t get_seg_free_list_idx(size_t size) {
  size_t idx = size / ALIGNMENT;
  if (idx >= NUM_LISTS) {
    idx = NUM_LISTS - 1;
  }
  return idx;
}

static void *mmap_helper(size_t size) {
  void *addr =
      mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
  if (addr == MAP_FAILED) {
    return NULL;
  }
  // successful mmap
  mem_pool_size += size;
  return addr;
}

static void *expand_memory(size_t size) {
  size_t needed_size = ALIGN(size);
  if (needed_size < PAGE_SIZE) {
    // PAGE is the smallest denomination of memory we can deal in with mmap
    needed_size = PAGE_SIZE;
  }

  void *addr = mmap_helper(needed_size);
  if (!addr) {
    return NULL;
  }

  // finally, create the block and put at the start of mempool
  m_block *block = (m_block *)addr;
  block->size = needed_size - sizeof(m_block);
  block->free = 1;
  block->next = mem_pool;
  mem_pool = block;
  return block;
}

void *malloc(size_t size) {
  if (size == 0) {
    return NULL;
  }
  pthread_mutex_lock(&seg_free_list_mutex);

  size_t idx = get_seg_free_list_idx(size);
  m_block *block = seg_free_list[idx];

  while (block) {
    if (block->size >= size) {
      // found block of suitable size in the list
      seg_free_list[idx] = block->next;
      block->free = 0;
      block->next = NULL;
      used_mem += block->size;
      pthread_mutex_unlock(&seg_free_list_mutex);
      return block->data;
    }
    block = block->next;
  }

  // did not find in list, need to allocate memory from system
  if (!mem_pool || mem_pool->size < size) {
    block = expand_memory(size);
    if (!block) {
      pthread_mutex_unlock(&seg_free_list_mutex);
      return NULL;
    }
  }

  block = mem_pool;
  mem_pool = mem_pool->next;
  block->free = 0;
  used_mem += block->size;

  pthread_mutex_unlock(&seg_free_list_mutex);
  return block->data;
}

void free(void *ptr) {
  if (!ptr) {
    return;
  }
  pthread_mutex_lock(&seg_free_list_mutex);

  // extract the block ptr
  m_block *block = (m_block *)((char *)ptr - offsetof(m_block, data));
  size_t block_idx = get_seg_free_list_idx(block->size);

  // finally, set free block
  block->free = 1;
  block->next = seg_free_list[block_idx];
  seg_free_list[block_idx] = block;

  used_mem -= block->size;
  pthread_mutex_unlock(&seg_free_list_mutex);
}

void *realloc(void *ptr, size_t size) {
  if (!ptr) {
    return malloc(size);
  }
  if (size == 0) {
    free(ptr);
    return NULL;
  }

  // extract the block
  m_block *block = (m_block *)((char *)ptr - offsetof(m_block, data));
  size_t prev_size = block->size;

  // just return same ptr
  if (size <= prev_size) {
    return ptr;
  }

  void *realloced = malloc(size);
  if (!realloced) {
    return NULL;
  }

  // copy over to new place
  memcpy(realloced, ptr, prev_size);
  free(ptr);
  return realloced;
}

void *calloc(size_t count, size_t size) {
  size_t total_size = count * size;
  void *addr = malloc(total_size);
  if (addr) {
    memset(addr, 0, total_size);
  }
  return addr;
}
