#include <stdio.h>
#include <string.h>
#include "lib/collections.h"
#include "lib/log.h"
#include "compiler/test.h"

int main(int argc, char **argv)
{
    char *path;
    if (argc == 1)
        path = "tests/lex/valid00.ya";
    else if (argc == 2)
        path = argv[1];
    else
    {
        log_msg(LOG_ERR, NULL, NO_POS, "invalid usage");
        return -1;
    }

    FILE *f = fopen(path, "rb");
    if (f == NULL)
    {
        log_msg(LOG_ERR, NULL, NO_POS, "cannot open file");
        return -1;
    }

    dump_lex(f);
}
/*
mut buf = ' '
for i 0:20
    buf += chr(ord('0') + i) * 100 + ' ';

out buf;
*/