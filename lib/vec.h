#ifndef YA_COLLECTIONS_H
#define YA_COLLECTIONS_H
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* In bytes. */
#define VEC_BLK_SIZE 8

/* Data follows header on heap. */
typedef struct
{
    size_t len;
    size_t capacity;
} vec_t;

/* const vec_t *vec, type -> type * */
#define VEC_DATA(vec, type) ((type *)(((type *)(vec)) + sizeof(vec_t)))

/* Returns vec_t with one block allocated. */
vec_t *vec_new();
vec_t *vec_append(vec_t *vec, const void *data, size_t size);
bool vec_equals(const vec_t *vec1, const vec_t *vec2);

#endif