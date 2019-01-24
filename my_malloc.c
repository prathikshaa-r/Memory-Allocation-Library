#include <stdio.h>
#include <unistd.h>
#include <assert.h>

#include "my_malloc.h"

static blk_t * head = NULL;
static size_t heap_start = 0;
static size_t total_metadata = 0;
static blk_t * tail = NULL;

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
//  assert
}

// free expexts a valid alloced pointer
// todo: detect? random pointer which is not malloced pointer
void ff_free(void * us_p) { // get user pointer
  insert_free(BLK_P(us_p)); // insert freed block into free list
}

void bf_free(void * us_p) {
  insert_free(BLK_P(us_p)); // onsert freed block into free list
}

void insert_free(blk_t * blk_ptr) {
  // block before head
  if (blk_ptr < head) {
    head->prev = blk_ptr;
    blk_ptr->next = head;
    head = blk_ptr;
  }
  // block after head
  else{
    blk_t * curr = head;

    // iterative approach to adding the freed block into LL
    // store in sorted order of addresses
    while (curr != NULL) {
      if(curr < blk_ptr){
      // bloak at end of free list
        if (curr->next = NULL) {
          curr->next = blk_ptr;
          // blk_ptr->next = NULL;
          blk_ptr->prev = curr;
          tail = blk_ptr;
        }
      // block in the middle of free list
        else {
          if (curr->next > blk_ptr) {
            blk_ptr->next = curr->next;
            blk_ptr->prev = curr;
            curr->next->prev = blk_ptr;
            curr->next = blk_ptr;
          }
          else {
            curr = curr->next;
          }
        }

      }
    }
  }
}
