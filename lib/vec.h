#ifndef YA_COLLECTIONS_H
#define YA_COLLECTIONS_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../platform.h"

/* Data follows header on heap. */
typedef struct
{
    size_t len;
    pl_blk_size_t blks;
    pl_blk_size_t blk_size;
} vec_t;

/* const vec_t *vec, type -> type * */
#define VEC_DATA(vec, type) ((type *)(((type *)(vec)) + sizeof(vec_t)))

/* Returns vec_t with one block allocated. */
vec_t *vec_new(pl_blk_size_t blk_size);
vec_t *vec_resize(vec_t *vec, size_t new_len);
vec_t *vec_append(vec_t *vec, const void *data, size_t size);
bool vec_equals(const vec_t *vec1, const vec_t *vec2);

#endif