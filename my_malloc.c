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

blk_t * ff_search(size_t size) {
  blk_t * curr = head;
  while(curr != NULL){
    // ff block found
    if(curr->size >= size){
      split(curr, size);
      return curr;
    }
    curr = curr->next;
  }
  // ff block not found
  return curr; // NULL when not found
}

blk_t * bf_search(size_t size) {
  blk_t * bf_blk = NULL;
  size_t diff = 0;
  blk_t * curr = head;
  while (curr != NULL) {
    if (curr->size < size) {
      curr = curr->next;
      continue;
    }
    // same size bock found - break
    if(curr->size == size){
      return curr;
    }
    // every fitting block
    if (curr->size > size) {
      size_t temp_diff = curr->size - size;
      if((temp_diff < diff) || (bf_blk == NULL)){
        if (bf_blk == NULL) {assert(diff == 0;)}
        diff = temp_diff;
        bf_blk = curr;
      }
    }
    curr = curr->next;
  } // end of while
  // if block found, split
  split(bf_blk, size); // todo check if split needs pointer ref
  return bf_blk;
}

// receives pointer of memory to be malloced
// if there is extra space, splits the block and adds a free block back to LL
void spilt(blk_t * start_ptr, size_t size) {
  if (blk_ptr == NULL) || (start_ptr->size <= size + 2*BLKHD_SIZE){
    return;
  }
    blk_t split_ptr = (void *)start + BLKHD_SIZE + size;
    split_ptr->next = NULL
    split_ptr->prev = NULL
    split_ptr->size = start_ptr->size - size;
    start_ptr->size = size;
    insert_free(split_ptr);
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
          blk_ptr->prev = curr;
          assert(blk_ptr->next == NULL);
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
