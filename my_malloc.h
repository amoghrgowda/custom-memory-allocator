#ifndef MY_MALLOC_H
#define MY_MALLOC_H

#include<stddef.h> // for size_t - to represent the highest possible memory size of any object

//function prototype
void* custom_malloc(size_t size);
void custom_free(void* ptr);

#endif