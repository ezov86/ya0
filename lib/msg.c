#include "msg.h"

char *msgs[] = {
    [YA_LIB_ALLOC_FATAL_ERR] = "[lib]: memory allocation error",

    [YA_COMP_LEX_INVALID_CHAR] = "[lex] invalid character `%c`",
    [YA_COMP_LEX_INVALID_ESC] = "[lex] invalid escape sequence `\\%c`",
    [YA_COMP_LEX_INVALID_ESC_HEX] = "[lex] invalid hex escape `%s`",
    [YA_COMP_LEX_INVALID_INT] = "[lex] invalid integer",
    [YA_COMP_LEX_TOO_BIG_INT] = "[lex] int `%s` is too big",
    [YA_COMP_LEX_TOO_SMALL_INT] = "[lex] int `%s` is too small"
};
