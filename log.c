#include "log.h"

#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include <unistd.h>

int log_msg(severity_t sev, char *filename, pos_t pos, char *str, ...)
{
    va_list args;
    va_start(args, str);

    if (filename != NULL)
        printf("%s:", filename);

    if (pos.line != 0)
        printf("%d:%d:", pos.line, pos.col);

    char *s;
    switch (sev)
    {
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

    printf(" %s", s);

    printf(": ");
    vprintf(str, args);
    printf(".\n");
    va_end(args);
}

void print_str(string_t *str) {
    /* May be slow, but works on any platform. */
    char *c = STRING_DATA(str);
    for (size_t i = 0; i < str->len; i++) {
        putchar(c[i]);
    }
}
