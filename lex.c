#include "lex.h"

#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

pos_t lex_tok_pos;
token_t lex_tok;
bool lex_error;

static char *filename;
static uint16_t line, col;

static STRING source;

static lex_options_t options;

static char cur_c;
static int src_i;

static int tab_size;

/* Before newline. */
static int prev_col;

static bool cur_tok_error;

void lex_init(char *_filename, STRING _source, lex_options_t _options)
{
    lex_error = false;

    filename = _filename;
    line = 1;
    col = 1;

    source = _source;

    (void *)(source + 1);

    if (source->len > 0)
        cur_c = STR_DATA(_source)[0];
    else
        cur_c = 0;

    src_i = 0;

    options = _options;

    if (_options & LEX_TAB2)
        tab_size = 2;
    else if (_options & LEX_TAB8)
        tab_size = 8;
    else
        tab_size = 4;

    prev_col = 1;

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
            line++;
            prev_col = col;
            col = 1;
        }
        else
        {
            /* If prev_col == -1 then rewinding 2 lines back (not allowed). */
            assert(prev_col == -1);

            line--;
            col = prev_col;
            prev_col = -1;
        }
        break;

    case 0:
    case '\r':
        /* Ignore. */
        break;

    case '\t':
        tab_diff = tab_size - ((col - 1) % tab_size);
        if (forward)
            col += tab_diff;
        else
            col -= tab_diff;
        break;

    default:
        if (forward)
            col++;
        else
            col--;
        break;
    }
}

static void nextc()
{
    update_pos(true);

    if (++src_i >= source->len)
    {
        cur_c = 0;
        return;
    }

    cur_c = STR_DATA(source)[src_i];
}

static void backc()
{
    if (src_i <= 0)
    {
        cur_c = 0;
        return;
    }

    cur_c = STR_DATA(source)[--src_i];
    update_pos(false);
}

#define ERROR(fmt, ...)                                              \
    {                                                                \
        cur_tok_error = true;                                        \
        lex_error = true;                                            \
        log_msg(LOG_ERR, filename, lex_tok_pos, fmt, ##__VA_ARGS__); \
    }

#define SAVE_TOK(_lexeme, _val_type, _val_f, _val) \
    lex_tok.lexeme = _lexeme;                      \
    lex_tok.val_type = _val_type;                  \
    lex_tok.val._val_f = _val;

static bool eat_blank()
{
    if (options & LEX_INGORE_WS)
    {
        while (cur_c == ' ' || cur_c == '\t')
            nextc();

        return false;
    }

    STRING str = str_new();
    while (cur_c == ' ' || cur_c == '\t')
    {
        str_push(str, cur_c);
        nextc();
    }

    if (str->len == 0)
        return false;

    SAVE_TOK(LEX_BLANK, TOK_VAL_STR, s, str)

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

    if (options & LEX_INGORE_WS)
    {
        while (cur_c != '\n' && cur_c != 0)
            nextc();

        return false;
    }

    STRING str = str_new();
    while (cur_c != '\n' && cur_c != 0)
    {
        str_push(str, cur_c);
        nextc();
    }

    SAVE_TOK(LEX_COMMENT, TOK_VAL_STR, s, str)

    return true;
}

static bool eat_newline()
{
    if (cur_c != '\n')
        return false;

    nextc();

    if (options & LEX_INGORE_WS)
        return false;

    SAVE_TOK(LEX_NEWLINE, TOK_VAL_NONE, i, 0)

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

    STRING str = str_new();
    do
    {
        str_push(str, cur_c);
        nextc();
    } while (is_alpha(cur_c) || cur_c == '_' || is_dig(cur_c));

    SAVE_TOK(LEX_NAME, TOK_VAL_STR, s, str)

    return true;
}

static int64_t str_to_i(const char *str, int base)
{
    char *endptr;
    long long result = strtoll(str, &endptr, base);

    if (errno == ERANGE)
    {
        if (result == LLONG_MAX)
            ERROR("`%s` is too big for int64", result)
        else if (result == LLONG_MIN)
            ERROR("`%s` is too small for int64", result)
        return 0;
    }

    if (endptr == str || *endptr != '\0')
    {
        ERROR("`%s` is invalid integer", result)
        return 0;
    }

    return result;
}

static void escape_seq(STRING str)
{
    /* Number. */
    if (cur_c == 'x')
    {
        char hex[3];
        nextc();
        hex[0] = cur_c;
        nextc();
        hex[1] = cur_c;
        hex[2] = 0;

        if (options & LEX_UNESCAPE_STR)
        {
            /* Overflow check is not needed (2 hex digits - max is 255). */
            uint8_t i = str_to_i(hex, 16);
            str_push(str, i);
        }
        else
        {
            str_append(str, "\\x", 2);
            str_append(str, hex, 2);
        }

        return;
    }

    /* Else character escape. */

    if (!(options & LEX_UNESCAPE_STR))
    {
        str_append(str, "\\", 2);
        str_push(str, cur_c);
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
        ERROR("invalid escape sequence `\\%c`", cur_c)
        return;
    }
#undef CASE

    str_push(str, unescaped);
}

static bool eat_string()
{
    if (cur_c != '\'' && cur_c != '"')
        return false;

    char quote = cur_c;

    nextc();

    STRING str = str_new();
    while (cur_c != quote)
    {
        switch (cur_c)
        {
        case 0:
            ERROR("unexpected EOF");
            return true;

        case '\n':
            ERROR("unexpected EOL");
            return true;

        case '\\':
            nextc();
            escape_seq(str);
            break;

        default:
            str_append(str, &cur_c, sizeof(char));
        }

        nextc();
    }

    /* Closing quote. */
    nextc();

    SAVE_TOK(LEX_STRING, TOK_VAL_STR, s, str)

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
    lex_tok_pos.line = line;
    lex_tok_pos.col = col;

    RULE(eat_blank, LEX_BLANK)
    RULE(eat_comment, LEX_COMMENT)
    RULE(eat_newline, LEX_NEWLINE)
    RULE(eat_name, LEX_NAME)
    RULE(eat_string, LEX_NAME)

    if (cur_c == 0)
        lex_tok.lexeme = LEX_END;
    else
    {
        ERROR("unknown character `%c`", cur_c);
        /* Skip character. */
        nextc();
        goto repeat;
    }

end:
    return;
}
