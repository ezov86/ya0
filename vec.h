#ifndef YA_BUF_H
#define YA_BUF_H

#include <stddef.h>

/* Динамический массив, размеры, вместимость и другое находится на стеке, а данные на куче. */
typedef struct
{
    size_t len;
    size_t capacity;
    size_t blk_size;
    void *ptr;
} svec_t;

#define SVEC_T(BLK_SIZE)                                           \
    (svec_t)                                                       \
    {                                                              \
        .blk_size = BLK_SIZE, .len = 0, .capacity = 0, .ptr = NULL \
    }

void svec_append(svec_t *buf, char *data, size_t size);
void svec_free(svec_t *buf);

#endif