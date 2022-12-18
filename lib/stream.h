#ifndef YA_STREAM_H
#define YA_STREAM_H

/* Data stream that can both work with memory data and <stdio.h> FILE. All functions works similar to the standard ones,
   except that no 'text' mode for memory stream is availabel. */

#include <stdio.h>
#include <stddef.h>
#include "vec.h"

typedef enum
{
    STREAM_FILE,
    STREAM_MEM
} stream_data_type_t;

typedef struct
{
    union
    {
        FILE *f;
        vec_t *vec;
    } data;
    size_t vec_i;
    stream_data_type_t data_type;
} stream_t;

/* FILE *file */
#define STREAM_FROM_FILE(file) ((stream_t){.data.f = file, .data_type = STREAM_FILE})
/* vec_t *vec
   Vector size should not be bigger than LONG_MAX, vector should not be changed during reading/writing. */
#define STREAM_FROM_VEC(vec) ((stream_t){.data.vec = vec, .val.vec_i = 0, .data_type = STREAM_MEM})

int stream_getc(stream_t *stream);
int stream_putc(int ch, stream_t *stream);
/* Reads size*count bytes for vector instead of reading `count` elements with given size. */
size_t stream_read(void *buf, size_t size, size_t count, stream_t *stream);
/* Writes size*count bytes for vector instead of writing `count` elements with given size. */
size_t stream_write(const void *buf, size_t size, size_t count, stream_t *stream);
long stream_tell(stream_t *stream);
int stream_seek(stream_t *stream, long offset, int origin);
/* Frees memory if vector was given. */
int stream_close(stream_t *stream);

#endif