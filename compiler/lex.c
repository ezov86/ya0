#include "lex.h"

#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "../lib/str.h"
#include "../log.h"
#include "../msg.h"

pos_t lex_tok_pos;
token_t lex_tok;
bool lex_error;

static char *filename;
static pos_t pos;

static stream_t *src_stream;

static lex_options_t options;

static char cur_c;
static int src_i;

static int tab_size;

/* Before newline. */
static int prev_col;

static bool cur_tok_error;

void lex_init(stream_t *_src_stream, lex_options_t _options)
{
    lex_error = false;

    pos.line = 1;
    pos.col = 1;

    src_stream = _src_stream;

    /* Peeking first character. */
    int ic = stream_getc(src_stream);
    if (ic == EOF)
        cur_c = 0;
    else
        cur_c = ic;

    src_i = 0;

    options = _options;

    if (_options & LEX_TAB2)
        tab_size = 2;
    else if (_options & LEX_TAB8)
        tab_size = 8;
    else
        tab_size = 4;

    prev_col = -1;

    cur_tok_error = false;
}

static void update_pos(bool forward)
{
    /* For tabulation. Declared here to follow ISO C99. */
    int tab_diff;

    switch (cur_c)
    {
    case '\n':
        if (forward)
        {
            pos.line++;
            prev_col = pos.col;
            pos.col = 1;
        }
        else
        {
            /* If prev_col == -1 then rewinding 2 lines back (not allowed). */
            assert(prev_col == -1);

            pos.line--;
            pos.col = prev_col;
            prev_col = -1;
        }
        break;

    case 0:
    case '\r':
        /* Ignore. */
        break;

    case '\t':
        tab_diff = tab_size - ((pos.col - 1) % tab_size);
        if (forward)
            pos.col += tab_diff;
        else
            pos.col -= tab_diff;
        break;

    default:
        if (forward)
            pos.col++;
        else
            pos.col--;
        break;
    }
}

static void nextc()
{
    update_pos(true);

    int ic = stream_getc(src_stream);
    if (ic == EOF)
    {
        cur_c = 0;
        return;
    }

    cur_c = ic;
}

static void backc()
{
    if (stream_tell(src_stream) == 0)
    {
        /* Begin of file, so we ran out of it. */
        cur_c = 0;
        return;
    }

    stream_seek(src_stream, -1, SEEK_CUR);
    int ic = stream_getc(src_stream);
    if (ic == EOF)
    {
        /* Strange situation happened. */
        cur_c = 0;
        return;
    }

    /* ^^ getc() moves cursor forward in file stream, so go back again. */
    stream_seek(src_stream, -1, SEEK_CUR);

    cur_c = ic;

    update_pos(false);
}

/* Be careful in `if` statements. */
#define ERROR(msg_id, ...)                                \
    {                                                     \
        cur_tok_error = true;                             \
        lex_error = true;                                 \
        log_add_msg(LOG_ERR, pos, msg_id, ##__VA_ARGS__); \
    }

static void save_tok(lexeme_t lexeme, lex_val_type_t val_type, void *value)
{
    lex_tok.lexeme = lexeme;
    lex_tok.val_type = val_type;

    switch (val_type)
    {
    case TOK_VAL_INT64:
        lex_tok.val.i = *(int64_t *)value;
        break;

    case TOK_VAL_STR:
        lex_tok.val.s = (string_t *)value;
        break;

    case TOK_VAL_FLOAT:
        lex_tok.val.f = *(float *)value;
        break;

    case TOK_VAL_NONE:
        break;

    default:
        assert(0);
    }
}

static bool eat_blank()
{
    string_t *str = string_new();
    while (cur_c == ' ' || cur_c == '\t')
    {
        str = string_push(str, cur_c);
        nextc();
    }

    if (str->len == 0)
        return false;

    save_tok(LEX_BLANK, TOK_VAL_STR, str);

    return true;
}

static bool eat_comment()
{
    if (cur_c != '/')
        return false;

    nextc();
    if (cur_c != '/')
    {
        backc();
        return false;
    }

    nextc();

    string_t *str = string_new();
    while (cur_c != '\n' && cur_c != 0)
    {
        str = string_push(str, cur_c);
        nextc();
    }

    save_tok(LEX_COMMENT, TOK_VAL_STR, str);

    return true;
}

static bool eat_newline()
{
    if (cur_c != '\n')
        return false;

    nextc();

    save_tok(LEX_NEWLINE, TOK_VAL_NONE, 0);

    return true;
}

static bool is_alpha(char c)
{
    return 'a' <= c && c <= 'z' || 'A' <= c && c <= 'Z';
}

static bool is_dig(char c)
{
    return '0' <= c && c <= '9';
}

static bool eat_name()
{
    if (!(is_alpha(cur_c) || cur_c == '_'))
        return false;

    string_t *str = string_new();
    do
    {
        str = string_push(str, cur_c);
        nextc();
    } while (is_alpha(cur_c) || cur_c == '_' || is_dig(cur_c));

    save_tok(LEX_NAME, TOK_VAL_STR, str);

    return true;
}

static int64_t str_to_i(string_t *str, int base)
{
    char *endptr;
    long long result = strtoll(STRING_DATA(str), &endptr, base);

    if (errno == ERANGE)
    {
        string_t *cutted_str = log_prepare_str(str);
        if (result == LLONG_MAX)
        {
            ERROR(YAC_LEX_TOO_BIG_INT, cutted_str);
        }
        else
        {
            ERROR(YAC_LEX_TOO_SMALL_INT, cutted_str);
        }

        free(cutted_str);
        return 0;
    }

    if (endptr == STRING_DATA(str) || *endptr != '\0')
    {
        string_t *cutted_str = log_prepare_str(str);
        ERROR(YAC_LEX_INVALID_INT, result);
        free(cutted_str);
        return 0;
    }

    return result;
}

static void escape_seq(string_t *str)
{
    /* Number. */
    if (cur_c == 'x')
    {
        string_t *hex = string_new();
        nextc();
        hex = string_push(hex, cur_c);
        nextc();
        hex = string_push(hex, cur_c);
        hex = string_push(hex, "\0");

        if (options & LEX_UNESCAPE_STR)
        {
            /* Overflow check is not needed (2 hex digits - max is 255). */
            uint8_t i = str_to_i(hex, 16);
            str = string_push(str, i);
        }
        else
        {
            str = string_append(str, "\\x", 2);
            str = string_append(str, STRING_DATA(hex), 2);
        }

        free(hex);

        return;
    }

    /* Else character escape. */

    if (!(options & LEX_UNESCAPE_STR))
    {
        str = string_append(str, "\\", 2);
        str = string_push(str, cur_c);
        return;
    }

    char unescaped;

#define CASE(c, e)     \
    case c:            \
        unescaped = e; \
        break;

    switch (cur_c)
    {
        CASE('0', 0);
        CASE('a', '\a')
        CASE('b', '\b')
        CASE('f', '\f')
        CASE('n', '\n')
        CASE('t', '\t')
        CASE('r', '\r')
        CASE('v', '\v')
        CASE('\'', '\'')
        CASE('"', '"')
        CASE('\\', '\\')
    default:
        ERROR(YAC_LEX_INVALID_ESC, cur_c);
        return;
    }
#undef CASE

    str = string_push(str, unescaped);
}

static bool eat_string()
{
    if (cur_c != '\'' && cur_c != '"')
        return false;

    char quote = cur_c;

    nextc();

    string_t *str = string_new();
    while (cur_c != quote)
    {
        switch (cur_c)
        {
        case 0:
            ERROR(YAC_LEX_UNEXPECTED_EOF);
            return true;

        case '\n':
            ERROR(YAC_LEX_UNEXPECTED_EOL);
            return true;

        case '\\':
            nextc();
            escape_seq(str);
            break;

        default:
            str = string_push(str, cur_c);
        }

        nextc();
    }

    /* Closing quote. */
    nextc();

    save_tok(LEX_STRING, TOK_VAL_STR, str);

    return true;
}

#define RULE(f, lexeme)                                \
    if (((lexeme & expected) || expected == 0) && f()) \
    {                                                  \
        if (cur_tok_error)                             \
            goto repeat;                               \
        else                                           \
            goto end;                                  \
    }

void lex_next(lexeme_t expected)
{
repeat:
    cur_tok_error = false;

    /* Save starting position of token. */
    lex_tok_pos = pos;

    RULE(eat_blank, LEX_BLANK)
    RULE(eat_comment, LEX_COMMENT)
    RULE(eat_newline, LEX_NEWLINE)
    RULE(eat_name, LEX_NAME)
    RULE(eat_string, LEX_NAME)

    if (cur_c == 0)
        lex_tok.lexeme = LEX_END;
    else
    {
        ERROR(YAC_LEX_INVALID_CHAR, cur_c);
        /* Skip character. */
        nextc();
        goto repeat;
    }

end:
    return;
}

void lex_free_token_val(token_t *tok)
{
    if (tok->val_type == TOK_VAL_STR)
        free(tok->val.s);
}
