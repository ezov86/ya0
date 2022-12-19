#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mem.h"
#include "../log.h"

void *safe_calloc(size_t n, size_t s)
{
    // printf("calloc %d*%d=%d\n", n, s, n * s);
    void *p = calloc(n, s);
    if (p == NULL)
    {
        /* TODO: jump to exception handler using <setjmp.h>. */
        puts("alloc error");
        exit(-1);
    }

    return p;
}

void *safe_malloc(size_t s)
{
    // printf("malloc %d\n", s);
    void *p = malloc(s);
    if (p == NULL)
    {
        /* TODO: jump to exception handler using <setjmp.h>. */
        puts("alloc error");
        exit(-1);
    }

    return p;
}

void *safe_realloc(void *ptr, size_t s)
{
    // printf("realloc %d\n", s);
    void *p = realloc(ptr, s);

    if (p == NULL)
    {
        /* TODO: jump to exception handler using <setjmp.h>. */
        puts("alloc error");
        exit(-1);
    }

    return p;
}
