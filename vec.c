#include "collections.h"

#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "mem.h"

void vec_append(vec_t *vec, void *data, size_t size)
{
    size_t prev_len = vec->len;
    vec->len += size;
    if (vec->len >= vec->capacity)
    {
        vec->capacity = (vec->len / VEC_BLK_SIZE + 1) * VEC_BLK_SIZE;
        vec->ptr = safe_realloc(vec->ptr, vec->capacity);
    }

    memcpy((void *)((char *)vec->ptr + prev_len), data, size);
}
