#include <stdio.h>
#include <string.h>
#include "lib/str.h"
#include "lib/stream.h"
#include "log.h"
#include "compiler/test.h"

int main(int argc, char **argv)
{
    char *path;
    if (argc == 1)
        path = "/home/tim/ya/tests/lex/valid00.ya";
    else if (argc == 2)
        path = argv[1];
    else
    {
        puts("invalid usage");
        return -1;
    }

    FILE *f = fopen(path, "rb");
    if (f == NULL)
    {
        puts("cannot open file");
        return -1;
    }

    stream_t stream = STREAM_FROM_FILE(f);

    log_init(NULL, stderr);
    dump_lex(&stream);
}
/*
mut buf = ' '
for i 0:20
    buf += chr(ord('0') + i) * 100 + ' ';

out buf;
*/