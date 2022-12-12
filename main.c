#include <stdio.h>
#include <string.h>
#include "vec.h"
#include "log.h"
#include "test.h"

int main(int argc, char **argv)
{
    // svec_t *buf = buf_new(sizeof(char), 8);

    // for (int j = 0; j < 20; j++)
    // {
    //     char buf2[100];
    //     for (int i = 0; i < 100; i++)
    //         buf2[i] = '0' + j;

    //     buf_append(buf, buf2, 100);
    //     buf_append(buf, " ", 1);
    // }

    // buf_append(buf, "\0", 1);
    // printf("%s %d %d %d\n", buf->ptr, strlen(buf->ptr), buf->len, buf->capacity);
    FILE *f = fopen("/home/tim/ya/tests/lexer/valid00.ya", "r");
    if (f == NULL) {
        log_msg(LOG_ERR, NULL, NO_POS, "cannot open file");
        return -1;
    }

    svec_t buf = SVEC_T(512);
    int c;
    while ((c = fgetc(f)) != EOF) {
        char cc = c;
        svec_append(&buf, &cc, sizeof(char));
    }

    dump_lex(buf);

    svec_free(&buf);
}
/*
mut buf = '';
buf += ' ';
for i 0:20
    buf += chr(ord('0') + i) * 100 + ' ';

out buf;
*/