#ifndef YA_LEXER_H
#define YA_LEXER_H

#include <stdint.h>
#include <stdbool.h>
#include "collections.h"
#include "log.h"

typedef enum
{
    LEX_BLANK = 1,
    LEX_COMMENT = 1 << 1,
    LEX_NEWLINE = 1 << 2,
    LEX_KEYWORD = 1 << 3,
    LEX_NAME = 1 << 4,
    LEX_PUNC = 1 << 5,
    LEX_STRING = 1 << 6,
    LEX_INT = 1 << 7,
    LEX_FLOAT = 1 << 8,
    LEX_END = 1 << 9,

    LEX_ANY = 0
} lexeme_t;

typedef enum
{
    TOK_VAL_NONE,
    TOK_VAL_INT64,
    TOK_VAL_FLOAT,
    TOK_VAL_STR
} lex_val_type_t;

typedef struct
{
    union
    {
        int64_t i;
        float f;
        string_t *s;
    } val;
    lex_val_type_t val_type;
    lexeme_t lexeme;
} token_t;

typedef enum
{
    KEYWORD_IF,
    KEYWORD_ELIF,
    KEYWORD_ELSE,
    KEYWORD_LOOP,
    KEYWORD_FOR,
    KEYWORD_RET,
    KEYWORD_BREAK,
    KEYWORD_CONT,
    KEYWORD_FN,
    KEYWORD_CLASS,
    KEYWORD_IMPORT,
    KEYWORD_MUT,
    KEYWORD_EXCEPT,
    KEYWORD_LET,
    KEYWORD_RAISE,
    KEYWORD_MATCH,
    KEYWORD_PUB
} keyword_t;

typedef enum
{
    /* ,   */ PUNC_COMMA = 0x0,
    /* .   */ PUNC_DOT = 0x1,
    /* ;   */ PUNC_SEMICOLON = 0x2,
    /* ->  */ PUNC_ARROW = 0x3,
    /* {   */ PUNC_LBRACE = 0x4,
    /* }   */ PUNC_RBRACE = 0x5,
    /* (   */ PUNC_LPAR = 0x6,
    /* )   */ PUNC_RPAR = 0x7,
    /* !   */ PUNC_NOT = 0x8,
    /* @   */ PUNC_LEN = 0x9,
    /* $   */ PUNC_DOLLAR = 0xA,
    /* ^   */ PUNC_POW = 0xB,
    /* |   */ PUNC_PIPE = 0xC,
    /* |>  */ PUNC_CONV = 0xD,
    /* +   */ PUNC_PLUS = 0xE,
    /* -   */ PUNC_MINUS = 0xF,
    /* *   */ PUNC_ASTERISK = 0x10,
    /* /   */ PUNC_DIV = 0x11,
    /* %   */ PUNC_MOD = 0x12,
    /* &&  */ PUNC_AND = 0x13,
    /* ||  */ PUNC_OR = 0x14,
    /* ++  */ PUNC_APPEND = 0x15,
    /* <   */ PUNC_LANGLE = 0x16,
    /* >   */ PUNC_RANGLE = 0x17,
    /* <=  */ PUNC_LS_EQ = 0x18,
    /* >=  */ PUNC_GR_EQ = 0x19,
    /* ==  */ PUNC_EQ = 0x1A,
    /* !=  */ PUNC_NEQ = 0x1B,
    /* :   */ PUNC_COLON = 0x1C,
    /* \   */ PUNC_LAMBDA = 0x1D,
    /* ... */ PUNC_PERIOD = 0x1E,

    /* Flags can be used with other punctuators. */
    PUNC_BIT_FLAG = 0x100,
    PUNC_ASSIGN_FLAG = 0x200
} punc_t;

extern pos_t lex_tok_pos;
extern token_t lex_tok;
extern bool lex_error;

typedef enum
{
    LEX_INGORE_WS = 1,
    LEX_UNESCAPE_STR = 1 << 1,

    /* Табуляция по 4 символа по-умолчанию. */
    LEX_TAB2 = 1 << 2,
    LEX_TAB8 = 1 << 3,

    LEX_PARSE_NUMS = 1 << 4,
} lex_options_t;

void lex_init(char *_filename, string_t *_source, lex_options_t _options);
void lex_next(lexeme_t expected);

#endif
