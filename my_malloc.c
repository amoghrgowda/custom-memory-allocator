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

// helper macros:

// to calculate address of data-area from the block header address.
// basically, takes block_t arg and returns the pointer for immediate byte following the header.
// Need to return this pointer to user.
#define BLOCK_DATA_ADDR(block_ptr) ((void*)((char*)(block_ptr)+sizeof(block_t)))

// calculate address of the block header.
// basically, reverse of BLOCK_DATA_ADDR() macro
#define GET_BLOCK_ADDR(block_ptr) ((void*)((char*)(block_ptr)-sizeof(block_t))) 

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

// helper function - to find a suitable free block using First fit.
block_t* find_free_block(size_t size){
    block_t *current = head;
    while(current != NULL){
        if(current->is_free && current->size >= size){
            return current;
        }
        current = current->next;
    }
    return NULL;
}

//custom_malloc implementation starts here
void custom_malloc(size_t size){
    if(size==0)
        return NULL;

    block_t *block;
    
    // for first call
    if(head==NULL){
        //the heap is empty, need to go to the first block
        block = extend_heap(size);
        if(block == NULL){
            // failed to extend the heap allocation
            return NULL; 
        }

        //set global head pointer
        head = block;

        return BLOCK_DATA_ADDR(block);
    }

    // search implementation for a free block using first fit
    block = find_free_block(size);

    if(block != NULL){
        block->is_free = 0;
        return BLOCK_DATA_ADDR(block);
    }

    // ATP no free block is found. Therefore, extend the heap.

    block_t *last = head;
    while(last->next != NULL){
        last = last->next;
    } 

    block = extend_heap(size);
    // checking if extension failed
    if(block == NULL){
        return NULL;
    }

    last->next = block;     //adding new block after the last one.
    return BLOCK_DATA_ADDR(block); 
}

//implementing custom_free here.

void custom_free(void *ptr){
    if(ptr==NULL){
        return;
    }

    block_t* block = GET_BLOCK_ADDR(ptr);

    // need to prevent user from double free
    if(block->is_free == 1){
        fprintf(stderr, "Warning :- Attempted to double free the block! \n");
        return;
    }

    block->is_free = 1;

    // need to merge different free blocks (coalescing)
    block_t* current = head;
    while(current != NULL){
        if(current->is_free && current->next != NULL && current->next->is_free){
            //Merging current and next block
            current->size += current->next->size + sizeof(block_t);

            // skip the link to the next block by linking current block to the one after next
            current->next = current->next->next;
        }
        current = current->next;
    }
}
