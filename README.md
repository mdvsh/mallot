### mallot

> c dynamic memory allocation experiments


---

#### current idea iterations
- basic malloc (backed by linked list of blocks)
- segregated free lists (for different block sizes)-
    - basic thread safety
    - alignment
    - memory protection
    - reuse freed blocks and split blocks to avoid fragmentation
- shift from sbrk to mmap
  - as former deprecated on macos
  - find a way to be not wasteful with mmap
    - implement mempool
  - also finish realloc, ccalloc
- add testing
