#include "stream.h"

#include <string.h>
#include <stdlib.h>

int stream_getc(stream_t *stream)
{
    if (stream->data_type == STREAM_FILE)
        return getc(stream->data.f);

    if (stream->vec_i >= stream->data.vec->len)
        return EOF;

    return VEC_DATA(stream->data.vec, char)[stream->vec_i++];
}

int stream_putc(int ch, stream_t *stream)
{
    if (stream->data_type == STREAM_FILE)
        return putc(ch, stream->data.f);

    vec_append(stream->data.vec, &ch, sizeof(char));
    stream->vec_i++;

    return ch;
}

size_t stream_read(void *buf, size_t size, size_t count, stream_t *stream)
{
    if (stream->data_type == STREAM_FILE)
        return fread(buf, size, count, stream->data.f);

    size_t _size = size * count;
    size_t rem = stream->data.vec->len - stream->vec_i;
    if (rem > _size)
        /* If vector has less elements than requested. */
        _size = rem;

    memcpy(buf, VEC_DATA(stream->data.vec, char), _size);
    stream->vec_i += _size;

    return _size;
}

size_t stream_write(const void *buf, size_t size, size_t count, stream_t *stream)
{
    if (stream->data_type == STREAM_FILE)
        return fwrite(buf, size, count, stream->data.f);

    size_t _size = size * count;
    vec_append(stream->data.vec, buf, _size);
    stream->vec_i += _size;

    return _size;
}

long stream_tell(stream_t *stream)
{
    if (stream->data_type == STREAM_FILE)
        return ftell(stream->data.f);

    return (long)stream->vec_i;
}

int stream_seek(stream_t *stream, long offset, int origin)
{
    if (stream->data_type == STREAM_FILE)
        return fseek(stream->data.f, offset, origin);

    switch (origin)
    {
    case SEEK_CUR:
        stream->vec_i += offset;
        break;

    case SEEK_SET:
        stream->vec_i = offset;
        break;

    case SEEK_END:
        stream->vec_i = stream->data.vec->len - offset;
        break;

    default:
        break;
    }

    return 0;
}

int stream_close(stream_t *stream)
{
    if (stream->data_type == STREAM_FILE)
        return fclose(stream->data.f);

    free(stream->data.vec);
}
