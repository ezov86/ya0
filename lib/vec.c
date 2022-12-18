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

vec_t *vec_append(vec_t *vec, const void *data, size_t size)
{
    size_t old_len = vec->len;
    vec->len += size;
    if (vec->len >= vec->capacity)
    {
        vec->capacity = (vec->len / VEC_BLK_SIZE + 1) * VEC_BLK_SIZE;
        vec = safe_realloc(vec, sizeof(vec_t) + vec->capacity);
    }

    memcpy((void *)((char *)vec + sizeof(vec_t) + old_len), data, size);

    /* If reallocation was done, returns new pointer. */
    return vec;
}

bool vec_equals(const vec_t *vec1, const vec_t *vec2)
{
    if (vec1->len != vec2->len)
        return false;

    char *d1 = VEC_DATA(vec1, char);
    char *d2 = VEC_DATA(vec2, char);
    for (size_t i = 0; i < vec1->len; i++)
        if (d1[i] != d2[i])
            return false;

    return true;
}
