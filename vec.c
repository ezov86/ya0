#include "vec.h"

#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "mem.h"

void svec_append(svec_t *vec, char *data, size_t size)
{
    assert(vec->blk_size != 0);

    size_t prev_len = vec->len;
    vec->len += size;
    if (vec->len >= vec->capacity)
    {
        // Перевыделение памяти.
        vec->capacity = (vec->len / vec->blk_size + 1) * vec->blk_size;
        vec->ptr = safe_realloc(vec->ptr, vec->capacity);
    }

    memcpy(vec->ptr + prev_len, data, size);
}

void svec_free(svec_t *vec) {
    vec->len = 0;
    vec->capacity = 0;
    free(vec->ptr);
}