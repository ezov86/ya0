#include "log.h"

#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "msg.h"
#include "lib/mem.h"

vec_t *log_messages;
string_t *log_file_path;

static stream_t *stream;

void log_init(string_t *_file_path, stream_t *_stream)
{
    if (_stream != NULL)
        log_messages = vec_new();
    else
        log_messages = NULL;

    log_file_path = _file_path;
    stream = _stream;
}

void log_destroy()
{
    free(log_messages);
}

void log_add_msg(log_severity_t severity, pos_t pos, msg_id_t msg_id, ...)
{
    char *msg_fmt = msgs[msg_id];
    char msg_buf[MSG_MAX_SIZE];
    size_t size = 0;

    va_list args;
    va_start(args, msg_id);

    if (log_file_path != NULL)
        size = sprintf(msg_buf, "%s:", log_file_path);

    if (pos.line != 0)
        size += sprintf(msg_buf + size, "%d:%d:", pos.line, pos.col);

    char *s;
    switch (severity)
    {
    case LOG_FATAL:
        s = "fatal";
        break;
    case LOG_ERR:
        s = "error";
        break;
    case LOG_WARN:
        s = "warning";
        break;
    case LOG_INFO:
        s = "info";
        break;
    case LOG_VERBOSE_INFO:
        s = "v_info";
        break;
    default:
        assert(0);
    }

    size += sprintf(msg_buf + size, " %s:", s);
    size += vsprintf(msg_buf + size, msg_fmt, args);
    msg_buf[size++] = '.';

    assert(size < MSG_MAX_SIZE);

    va_end(args);

    msg_buf[MSG_MAX_SIZE - 1] = 0;

    log_msg_t msg = {.severity = severity, .pos = pos, .id = msg_id};
    if (stream != NULL)
    {
        stream_write(msg_buf, 1, size, stream);
        stream_putc('\n', stream);
        /* No need to save string message if file was given. */
        msg.str = NULL;
    }
    else
    {
        char *allocated_buf = safe_malloc(sizeof(char) * size);

        memcpy(allocated_buf, msg_buf, sizeof(char) * size);
        msg.str = allocated_buf;
    }

    vec_append(log_messages, &msg, sizeof(log_msg_t));
}

string_t *log_prepare_str(string_t *str)
{
    string_t *escaped = string_escape(str);
    // if (escaped->len >=)
}
