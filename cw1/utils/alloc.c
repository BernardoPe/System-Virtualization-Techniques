#include "./alloc.h"
#include <stdlib.h>
#include <stdio.h>

void *safe_malloc(size_t size)
{
    void *ptr = malloc(size);
    if (ptr == NULL)
    {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

void *safe_realloc(void *ptr, size_t size)
{
    void *new_ptr = realloc(ptr, size);
    if (new_ptr == NULL)
    {
        perror("Failed to reallocate memory");
        exit(EXIT_FAILURE);
    }
    return new_ptr;
}