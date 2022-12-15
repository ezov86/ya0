#ifndef LOG_H
#define LOG_H

#include <stdint.h>
#include "collections.h"

typedef enum
{
    LOG_ERR,
    LOG_WARN,
    LOG_INFO,
    LOG_VERBOSE_INFO
} severity_t;

typedef struct
{
    uint16_t line;
    uint16_t col;
} pos_t;

#define NO_POS              \
    (pos_t)                 \
    {                       \
        .line = 0, .col = 0 \
    }

int log_msg(severity_t sev, char *filename, pos_t pos, char *str, ...);
/* Print string_t with its lenght, doesn't stop on zeros. */
void print_str(string_t *str);

#endif