#include "msg.h"

char *msgs[] = {
    [YA_LIB_ALLOC_FATAL_ERR] = "[lib]: memory allocation error",

    [YAC_LEX_INVALID_CHAR] = "[lex] invalid character `%c`",
    [YAC_LEX_INVALID_ESC] = "[lex] invalid escape sequence `\\%c`",
    [YAC_LEX_INVALID_INT] = "[lex] invalid integer",
    [YAC_LEX_TOO_BIG_INT] = "[lex] int is too big `%s`",
    [YAC_LEX_TOO_SMALL_INT] = "[lex] int is too small `%s`",
    [YAC_LEX_UNEXPECTED_EOL] = "[lex] unexpected end of line",
    [YAC_LEX_UNEXPECTED_EOF] = "[lex] unexpected end of file",
};
