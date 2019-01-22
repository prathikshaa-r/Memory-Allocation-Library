#include <stdio.h>
#include <unistd.h>
#include <assert.h>

#include "my_malloc.h"

blk_t * head = NULL;
size_t heap_start = 0;
size_t total_metadata = 0;
blk_t * tail = NULL;

int main(void) {
  void *ptr = sbrk(0);
  printf("Program Break: %p\n", ptr);
}

void * ff_malloc(size_t size) { // input - bytes
  //blk_t * blk_ptr
  if (head==NULL) {
    // heap_start = sbrk(0)
    // get memory sbrk()
    // head = sbrk(size + sizeof())
    // call grow_heap(size)
  }
  else {
    // search block
    // for loop

  }
  return blk_ptr + BLKHD_SIZE;
}

void * grow_heap(blk_t * prev, size_t inc_size){
  blk_t * new_blk = sbrk(0);
  void * alloc = sbrk(inc_size + BLKHD_SIZE);
  assert
}
