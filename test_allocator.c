#include "my_malloc.h"
#include<stdio.h>
int main(){
    printf("Custom memory allocator is running...\n");
    int* a = (int*)custom_malloc(sizeof(int));
    if(a)
        *a = 500;
    printf("Allocated block 'a' with value %d at memory address %p.\n",*a, (void*)a);

    // testing potential heap allocation
    char *b = (char *)custom_malloc(50);
    if (b) {
        b[0] = 'H';
        b[49] = 'P';
        printf("Allocated 50 bytes at %p.\n", (void *)b);
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

    // Test 5: Clean up
    custom_free(b);
    custom_free(c);

    printf("Freed block 'b' and block 'c' successfully.\nExiting the custom memory allocator.");
    return 0;
}