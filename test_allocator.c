#include "my_malloc.h"
#include<stdio.h>
int main(){
    printf("Custom memory allocator is running...\n");
    int* a = (int*)custom_malloc(sizeof(int));
    if(a)
        *a = 100;
    printf("Allocated block 'a' with value %d at memory address %p.\n",*a, (void*)a);

    // testing potential heap allocation
    char *b = (char *)custom_malloc(50);
    if (b) {
        b[0] = 'H';
        b[49] = 'P';
        printf("Allocated 50 bytes to block 'b' at %p.\n", (void *)b);
    }

    // Freeing a block
    custom_free(a);
    printf("Freed block 'a'.\n");
    
    // Re-allocation Test(should use the space freed by 'a' if size matches)
    int *c = (int *)custom_malloc(sizeof(int));
    if (c) {
        *c = 200;
        // This address should be very close to or the same as the address of 'a'
        printf("Re-allocated int at location %p with value: %d. Renaming block to 'c'.\n", (void *)c, *c); 
    }

    // Clean up of block b and c.
    custom_free(b);
    custom_free(c);

    printf("Freed block 'b' and block 'c' successfully.\n");

/*     
TESTING FOR BLOCK SPLIT FUNCTIONALITY

 // Allocate a large block (B1)
    size_t size_B1 = 200;
    void *B1 = custom_malloc(size_B1);
    printf("Allocated %zu bytes (B1) at %p\n", size_B1, B1);

    //Free the large block (B1). This block is now our large free "victim."
    custom_free(B1);
    printf("Freed B1. A large free block is ready for splitting.\n");
    
    // Request a small size (S1). This should force a SPLIT of B1.
    size_t size_S1 = 10;
    void *S1 = custom_malloc(size_S1);
    printf("\nAllocated %zu bytes (S1) at %p\n", size_S1, S1);
    
    printf("   Observation: S1 address should match B1 address, proving reuse.\n");
    
    // Request a small size again (S2). This should reuse S1's space if S1 is freed.
    custom_free(S1);
    printf("Freed S1.\n");
    
    void *S2 = custom_malloc(size_S1);
    printf("Allocated %zu bytes (S2) at %p\n", size_S1, S2);
    
    printf("   Observation: S2 address should match S1 address, proving simple reuse/coalescing.\n");

    custom_free(S2);
 */
    printf("Freed all blocks. Exiting the custom memory allocator.");
    return 0;
}