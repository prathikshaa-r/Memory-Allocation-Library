#include <stdio.h>
#include <unistd.h>
#include <assert.h>

#include "my_malloc.h"

static blk_t * head = NULL;
static void * heap_start = 0;
static size_t total_metadata = 0;
static blk_t * tail = NULL;

int main(void) {
  void *ptr = sbrk(0);
  printf("Program Break: %p\n", ptr);
}

unsigned long get_data_segment_size(){
  return sbrk(0) - heap_start;
}

unsigned long get_data_segment_free_space_size(){
  blk_t * curr = head;
  size_t free_space = 0;
  while (curr != NULL) {
    free_space += curr->size + BLKHD_SIZE;
    curr = curr->next;
  }
  return free_space;
}

void * ff_malloc(size_t size) { // input - bytes
  return my_malloc(size, 1);
}

void * bf_malloc(size_t size){
  return my_malloc(size, 0);
}

void * my_malloc(size_t size, int ff){
  // ptr malloced for use
  blk_t * blk_ptr = NULL;
  if (head == NULL) {
    assert(tail == NULL);
    heap_start = sbrk(0);
    blk_ptr = getmem(size);
  }
  else {
    // search block
    switch (ff) {
      case 0: // bf
        blk_ptr = bf_search(size);
        break;
      case 1: // ff
        blk_ptr = ff_search(size);
        break;
      // todo: default - print error
    }
    // block not found
    if (blk_ptr == NULL){
      blk_ptr = getmem(size);
    }
    // block found
    else{
      // search functions return block after split
      remove_free(blk_ptr);
    }
  }
  return US_P(blk_ptr);
}

blk_t * getmem(size_t size) {
  blk_t * new_blk = sbrk(4*(BLKHD_SIZE+size));
  new_blk->size = 4*(BLKHD_SIZE+size) - BLKHD_SIZE;
  new_blk->next = NULL;
  new_blk->prev = tail;
  tail = new_blk;
  split(new_blk, size);
  remove_free(new_blk);
  return new_blk;
}

void remove_free(blk_t * blk_ptr){
  if((blk_ptr->next == NULL) && (blk_ptr->prev == NULL)) {
    return;
  } // todo: verify if split sets these
  // block at head
  if (head == blk_ptr) {
    assert(blk_ptr->prev == NULL);
    head = blk_ptr->next;
    head->prev = NULL;
  }
  // block at tail
  if (tail == blk_ptr) {
    assert(blk_ptr->next == NULL);
    tail = blk_ptr->prev;
    tail->next = NULL;
  }
  // block in the middle
  else{
    blk_ptr->prev->next = blk_ptr->next;
    blk_ptr->next->prev = blk_ptr->prev;
  }
  blk_ptr->prev = NULL;
  blk_ptr->next = NULL;
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
        if (bf_blk == NULL) {assert(diff == 0);}
        diff = temp_diff;
        bf_blk = curr;
      }
    }
    curr = curr->next;
  } // end of while
  // if block found, split
  split(bf_blk, size);
  return bf_blk;
}

// receives pointer of memory to be malloced
// if there is extra space, splits the block and adds a free block back to LL
void split(blk_t * start_ptr, size_t size) {
  if ((start_ptr == NULL) || (start_ptr->size <= size + 2*BLKHD_SIZE)){
    return;
  }
    blk_t * split_ptr = (void *)start_ptr + BLKHD_SIZE + size;
    split_ptr->next = NULL;
    split_ptr->prev = NULL;
    split_ptr->size = start_ptr->size - size;
    start_ptr->size = size;
    insert_free(split_ptr);
}

// free expexts a valid alloced pointer
// todo: detect? random pointer which is not malloced pointer
void ff_free(void * us_p) { // get user pointer
  insert_free(BLK_P(us_p)); // insert freed block into free list
}

void bf_free(void * us_p) {
  insert_free(BLK_P(us_p)); // insert freed block into free list
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
      // block at end of free list
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
    } // end of while
  }
  merge(blk_ptr);
}

void merge(blk_t * blk_ptr){
  if (blk_ptr->prev == NULL){
    assert(blk_ptr == head);
  }
  // check adj with prev block
  else{
    if (blk_ptr == ((void *)US_P(blk_ptr->prev) + blk_ptr->prev->size)) {
      blk_ptr->prev->size = blk_ptr->prev->size + BLKHD_SIZE + blk_ptr->size;
      blk_ptr->prev->next = blk_ptr->next;
      blk_ptr->next->prev = blk_ptr->prev;
      blk_ptr->size = 0;
      if (tail == blk_ptr) {
        assert(blk_ptr->next == NULL);
        tail = blk_ptr->prev;
      }
      // todo: how to clear info prev, next of middle block
      blk_ptr = blk_ptr->prev;
      // todo: update tail if curr is tail
    }
  }

  if (blk_ptr->next == NULL) {
    assert(blk_ptr == tail);
  }
  // check with next block
  else{
    if (blk_ptr->next == ((void *)US_P(blk_ptr) + blk_ptr->size)) {
      blk_ptr->size = blk_ptr->size + BLKHD_SIZE + blk_ptr->next->size;
      // done: update tail if next is tail
      if(blk_ptr->next == tail){
        assert(blk_ptr->next->next == NULL);
        tail = blk_ptr;
      }
      blk_ptr->next = blk_ptr->next->next;
      blk_ptr->next->prev = blk_ptr;
    }
  }
}
