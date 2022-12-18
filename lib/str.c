#include "str.h"

#include <ctype.h>

void string_fprint(FILE *f, string_t *str)
{
    /* May be slow, but works on any platform. */
    char *c = STRING_DATA(str);
    for (size_t i = 0; i < str->len; i++)
    {
        fputc(c[i], f);
    }
}

string_t *string_escape(string_t *str)
{
    string_t *esc_str = string_new();
    char *ptr = STRING_DATA(str);

    for (size_t i = 0; i <= str->len; i++)
    {
        if (isprint(ptr[i]))
        {
            esc_str = string_push(esc_str, ptr[i]);
            continue;
        }



#define CASE(c, esc_c)                    \
    case c:                               \
        string_append(esc_str, esc_c, 2); \
        break;

        /* Hex escape. 2 bytes for \x, 2 for num, 1 for sprintf to put null.
           Declared here for switch to follow ISO C. */
        char buf[5];

        switch (ptr[i])
        {
            CASE('\0', "\\0")
            CASE('\a', "\\a")
            CASE('\b', "\\b")
            CASE('\f', "\\f")
            CASE('\n', "\\n")
            CASE('\t', "\\t")
            CASE('\r', "\\r")
            CASE('\v', "\\v")
            CASE('\'', "\\'")
            CASE('\\', "\\\\")
        default:
            buf[0] = '\\';
            buf[1] = 'x';
            sprintf(buf, "%2x", ptr[i]);
            esc_str = string_append(esc_str, buf, 4);
            break;
        }

#undef CASE
    }

    return esc_str;
}
