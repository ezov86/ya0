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

#define VEC_DATA(vec, type) ((type *)(((char *)vec) + sizeof(vec_t)))

/* Returns vec_t with one block allocated. */
vec_t *vec_new();
vec_t *vec_append(vec_t *vec, void *data, size_t size);

typedef vec_t *STRING;

#define str_new() vec_new()
#define str_push(str, c) vec_append(str, &c, sizeof(char))
#define str_append(str, c_str, len) vec_append(str, c_str, (len) * sizeof(char))

#define STR_DATA(str) VEC_DATA(str, char)

#endif