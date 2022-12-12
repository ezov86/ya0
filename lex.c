#include "lex.h"

#include <assert.h>

#define TOK_VEC_BLK_SIZE 8
#define TOK_VEC SVEC_T(TOK_VEC_BLK_SIZE)

pos_t lex_tok_pos;
token_t lex_tok;
bool lex_error;

static char *filename;
static uint16_t line, col;

static char *text;
static size_t text_len;

static lex_options_t options;

static char cur_c;
static int text_i;

static int tab_size;

/* Номер последнего столбца перед '\n'. */
static int prev_col;
/* Ошибка в текущем токене. */
static bool cur_tok_error;

void lex_init(char *_filename, svec_t source_buf, lex_options_t _options)
{
    lex_error = false;

    filename = _filename;
    line = 1;
    col = 1;

    text = (char *)source_buf.ptr;
    text_len = source_buf.len;

    if (text_len > 0)
        cur_c = text[0];
    else
        cur_c = 0;

    text_i = 0;

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
            /* Если prev_col == -1, то происходит возвращение на 2 строки назад, что недопустимо. */
            assert(prev_col == -1);

            line--;
            col = prev_col;
            prev_col = -1;
        }

    case 0:
    case '\r':
        // Игнорируется.
        break;

    case '\t':
        int c = tab_size - ((col - 1) % tab_size); //- 1;
        if (forward)
            col += c;
        else
            col -= c;
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

    if (++text_i >= text_len)
    {
        cur_c = 0;
        return;
    }

    cur_c = text[text_i];
}

static void backc()
{
    if (text_i <= 0)
    {
        cur_c = 0;
        return;
    }

    cur_c = text[--text_i];
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

    svec_t vec = TOK_VEC;
    while (cur_c == ' ' || cur_c == '\t')
    {
        svec_append(&vec, &cur_c, sizeof(char));
        nextc();
    }

    if (vec.len == 0)
        return false;

    SAVE_TOK(LEX_BLANK, TOK_VAL_STR, s, vec.ptr);

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

    svec_t vec = TOK_VEC;
    while (cur_c != '\n' && cur_c != 0)
    {
        svec_append(&vec, &cur_c, sizeof(char));
        nextc();
    }

    SAVE_TOK(LEX_COMMENT, TOK_VAL_STR, s, vec.ptr);

    return true;
}

static bool eat_newline()
{
    if (cur_c != '\n')
        return false;

    nextc();

    if (options & LEX_INGORE_WS)
        return false;

    SAVE_TOK(LEX_NEWLINE, TOK_VAL_NONE, i, 0);

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

    svec_t vec = TOK_VEC;
    do
    {
        svec_append(&vec, &cur_c, sizeof(char));
        nextc();
    } while (is_alpha(cur_c) || cur_c == '_' || is_dig(cur_c));

    SAVE_TOK(LEX_NAME, TOK_VAL_STR, s, vec.ptr);

    return true;
}

static void escape_seq(svec_t *vec)
{
}

static bool eat_string()
{
    char quote;
    if (cur_c == '\'')
        quote = '\'';
    else if (cur_c == '"')
        quote = '"';
    else
        return false;

    nextc();

    svec_t vec = TOK_VEC;
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
            escape_seq(&vec);
            break;

        default:
            svec_append(&vec, &cur_c, sizeof(char));
        }

        nextc();
    }

    /* Закрывающая кавычка. */
    nextc();

    SAVE_TOK(LEX_STRING, TOK_VAL_STR, s, vec.ptr);

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

    /* Сохранение позиции начала токена. */
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
        /* Пропуск символа. */
        nextc();
        goto repeat;
    }

end:
}
