#ifndef YA_COLLECTIONS_H
#define YA_COLLECTIONS_H

#include <stddef.h>
#include <stdint.h>

/* In bytes. */
#define VEC_BLK_SIZE 8
/* If capacity equals to this macro, then no resize is available for vector. */
#define VEC_CONST_CAPACITY ((size_t)~0)

typedef struct
{
    size_t len;
    size_t capacity;
    void *ptr;
    /* For future use in VM with slices. */
    size_t step;
} vec_t;

#define VEC_NEW() ((vec_t){ .len = 0, .capacity = 0, .ptr = NULL})
#define VEC_FROM_PTR(ptr, len) ((vec_t){ .len = 0, .capacity = 0})
#define VEC_DATA(vec, type) ((type*)((vec).ptr))

void vec_append(vec_t *vec, void *data, size_t size);

typedef vec_t string_t;

#define STRING_NEW() VEC_NEW()
#define string_push(str, c) vec_append(str, &(c), sizeof(char))
#define string_append(str, c_str, len) vec_append(str, c_str, (len) * sizeof(char))

#define STRING_DATA(str) VEC_DATA(str, char)

#endif