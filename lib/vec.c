#include "vec.h"

#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "mem.h"

vec_t *vec_new()
{
    vec_t *vec = safe_malloc(sizeof(vec_t) + VEC_BLK_SIZE);
    vec->len = 0;
    vec->capacity = VEC_BLK_SIZE;
    return vec;
}

vec_t *vec_append(vec_t *vec, void *data, size_t size)
{
    size_t old_len = vec->len;
    vec->len += size;
    if (vec->len >= vec->capacity)
    {
        vec->capacity = (vec->len / VEC_BLK_SIZE + 1) * VEC_BLK_SIZE;
        vec = safe_realloc(vec, sizeof(vec_t) + vec->capacity);
    }

    memcpy((void *)((char*)vec + sizeof(vec_t) + old_len), data, size);

    /* If reallocation was done, returns new pointer. */
    return vec;
}