#ifndef H_MEM
#define H_MEM

#include <stddef.h>

void *safe_calloc(size_t n, size_t s);
void *safe_malloc(size_t s);
void *safe_realloc(void *ptr, size_t s);

#endif