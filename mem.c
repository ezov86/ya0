#include <stdio.h>
#include <stdlib.h>

#include "mem.h"
#include "log.h"

void *safe_calloc(size_t n, size_t s)
{
    void *p = calloc(n, s);
    if (p == NULL)
    {
        log_msg(LOG_ERR, NULL, NO_POS, "calloc error");
        exit(-1);
    }

    return p;
}

void *safe_malloc(size_t s)
{
    void *p = malloc(s);
    if (p == NULL)
    {
        log_msg(LOG_ERR, NULL, NO_POS, "malloc error");
        exit(-1);
    }

    return p;
}

void *safe_realloc(void *ptr, size_t s)
{
    void *p = realloc(ptr, s);
    if (p == NULL)
    {
        log_msg(LOG_ERR, NULL, NO_POS, "realloc error");
        exit(-1);
    }

    return p;
}
