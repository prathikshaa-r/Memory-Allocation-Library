# ECE-650-Homework-1

## Goal
Implementing `malloc()` and `free()` using `sbrk()` C wrapper on `brk()` system call. Implemented two verisons - first fit policy and best fit policy.

## Design
Implement a global free list which only tracks the free nodes. The free list is a doubly linked list of avalable or freed memory blocks. However, block metadata is still expected to be valid in malloced blocks and is used upon a call to `free()`. Total data segment size is tracked by saving the `heap_start` upon the first call to malloc (make sure not to reset it whenver the linked list is empty).

- The program merges adjacent memory locations.
- If a block at the end of the heap is free but none of the blocks in the free list are sufficient for the `malloc()` an `sbrk()` call is performed. However, this last memory segment is not coalesced and provided.
- The program checks to make sure that when a block is being provided to a `malloc()` if it is large enough, it is broken up into two nodes and the node of the required size is given to the user while the rest is put back into the free list as a new node.
- Memory alignment according to page size, etc. not done here.
