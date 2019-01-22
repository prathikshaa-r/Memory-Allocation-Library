/**
 * Implementing malloc and free using sbrk() - C wrapper on brk() syscall
 * Maintain a free list - doubly linked list which tracks free blocks
 *
 */

#include <unistd.h>

// create a doubly LL node struct to track free list
struct _blk_t {
  size_t size; // bytes
  struct _blk_t * next;
  struct _blk_t * prev;
} typedef blk_t;

#define BLKHD_SIZE sizeof(blk_t)

// First fit malloc/free
void * ff_malloc(size_t size);
void ff_free(void * ptr);

// Best fit malloc/free
void * bf_malloc(size_t size);
void bf_free(void * ptr);

unsigned long get_data_segment_size(); // in bytes
unsigned long get_data_segment_free_space_size(); // in bytes
