#include <stdio.h>
#include <string.h>
#include "collections.h"
#include "log.h"
#include "test.h"

int main(int argc, char **argv)
{
    FILE *f;
    char *filename;

    if (argc == 1)
    {
        f = fopen("tests/lex/valid00.ya", "r");
        if (f == NULL)
        {
            log_msg(LOG_ERR, NULL, NO_POS, "cannot open file");
            return -1;
        }
    }
    else if (argc == 2)
    {
        f = stdin;
    }
    else
    {
        log_msg(LOG_ERR, NULL, NO_POS, "invalid usage");
        return -1;
    }

    string_t *buf = str_new();
    int c;
    while ((c = fgetc(f)) != EOF)
    {
        char cc = c;
        buf = str_push(buf, cc);
    }

    dump_lex(buf);
}
/*
mut buf = ' '
for i 0:20
    buf += chr(ord('0') + i) * 100 + ' ';

out buf;
*/