// The comments in this code is not with consideration of other's understanding but 
// for reference by the creator(aka me) only.
#include<stdio.h>
#include<unistd.h> // for sbrk() - system call to change program break location in heap
#include<stddef.h> // for size_t - to represent the highest possible memory size of any object

// The following is a header for each memory block
typedef struct memory_block{
    size_t size;    // size of memory block following this header
    int is_free;    // Status: 0 = allocated, 1 = free
    struct memory_block *next; // pointer to next block in the linked list
} block_t;

//global heap pointer for our managed memory list:
static block_t *head = NULL;

// helper macro to calculate address of data-area from the block header address.
// basically, takes block_t arg and returns the pointer for immediate byte following the header.
// Need to return this pointer to user.
#define BLOCK_DATA_ADDR(block_ptr) ((void*)((char*)(block_ptr)+sizeof(block_t)))

//function prototype
void* custom_malloc(size_t size);
void custom_free(void* ptr);

//function to request new memory chunk from OS:
block_t* extend_heap(size_t size){
    block_t* block;

    //size of total memory needed to be requested:
    size_t required_size = size+sizeof(block_t);

    //move the program break up:
    block = (block_t*)sbrk(required_size);

    //checking for failure to extend the heap. sbrk returns pointer to -1:
    if (block == (void*)-1){
        return NULL;
    }

    block->size = size;
    block->is_free = 0;
    block->next = NULL;

    //This new block will be linked to END of the head list.
    // This will be done inside custom_malloc function.

    return block;
}