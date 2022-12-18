#ifndef YA_COLLECTIONS_H
#define YA_COLLECTIONS_H
#include <stddef.h>
#include <stdint.h>

/* In bytes. */
#define VEC_BLK_SIZE 8

/* Data follows header on heap. */
typedef struct
{
    size_t len;
    size_t capacity;
} vec_t;

#define VEC_DATA(vec, type) ((type *)(((type *)(vec)) + sizeof(vec_t)))

/* Returns vec_t with one block allocated. */
vec_t *vec_new();
vec_t *vec_append(vec_t *vec, void *data, size_t size);

#endif