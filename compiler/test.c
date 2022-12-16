#include "test.h"

#include <assert.h>
#include <stdio.h>
#include "lex.h"
#include "../lib/log.h"

#define CASE(a, b) \
    case (a):      \
        s = b;     \
        break;

#define F_OPER(flag, op1, op2) \
    if (tok.val.i & flag)      \
        s = op1;               \
    else                       \
        s = op2;               \
    break;

#define ASS_BIT_OPER(op1, op2, op3, op4)                               \
    if ((tok.val.i & PUNC_BIT_FLAG) && (tok.val.i & PUNC_ASSIGN_FLAG)) \
        s = op1;                                                       \
    else if (tok.val.i & PUNC_BIT_FLAG)                                \
        s = op2;                                                       \
    else if (tok.val.i & PUNC_ASSIGN_FLAG)                             \
        s = op3;                                                       \
    else                                                               \
        s = op4;

static void dump_token(token_t tok, pos_t pos)
{
    printf("%d:%d ", pos.line, pos.col);

    char *s;
    switch (tok.lexeme)
    {
        CASE(LEX_BLANK, "bl")
        CASE(LEX_COMMENT, "comm")
        CASE(LEX_NEWLINE, "nl")
        CASE(LEX_KEYWORD, "kw")
        CASE(LEX_NAME, "name")
        CASE(LEX_PUNC, "punc")
        CASE(LEX_STRING, "str")
        CASE(LEX_INT, "int")
        CASE(LEX_FLOAT, "float")
    default:
        assert(0);
    }

    printf("%s", s);

    if (tok.lexeme == LEX_KEYWORD)
    {
        switch (tok.val.i)
        {
            CASE(KEYWORD_IF, "if")
            CASE(KEYWORD_ELIF, "elif")
            CASE(KEYWORD_ELSE, "else")
            CASE(KEYWORD_LOOP, "loop")
            CASE(KEYWORD_FOR, "for")
            CASE(KEYWORD_RET, "ret")
            CASE(KEYWORD_BREAK, "break")
            CASE(KEYWORD_CONT, "cont")
            CASE(KEYWORD_FN, "fn")
            CASE(KEYWORD_CLASS, "class")
            CASE(KEYWORD_IMPORT, "import")
            CASE(KEYWORD_MUT, "mut")
            CASE(KEYWORD_EXCEPT, "except")
            CASE(KEYWORD_LET, "let")
            CASE(KEYWORD_RAISE, "raise")
            CASE(KEYWORD_MATCH, "match")
            CASE(KEYWORD_PUB, "pub")
        default:
            assert(0);
        }
    }
    else if (tok.lexeme == LEX_PUNC)
    {
        switch (tok.val.i)
        {
            CASE(PUNC_COMMA, ",")
            CASE(PUNC_DOT, ".")
            CASE(PUNC_SEMICOLON, ":")
            CASE(PUNC_ARROW, "->")
            CASE(PUNC_LBRACE, "{")
            CASE(PUNC_RBRACE, "}")
            CASE(PUNC_LPAR, "(")
            CASE(PUNC_RPAR, ")")
        case PUNC_NOT:
            F_OPER(PUNC_BIT_FLAG, "~", "!")
            CASE(PUNC_LEN, "@")
            CASE(PUNC_DOLLAR, "$")
            CASE(PUNC_POW, "^")
            CASE(PUNC_PIPE, "|")
            CASE(PUNC_CONV, "|>")
        case PUNC_PLUS:
            F_OPER(PUNC_ASSIGN_FLAG, "+=", "+")
        case PUNC_MINUS:
            F_OPER(PUNC_ASSIGN_FLAG, "-=", "-")
        case PUNC_ASTERISK:
            F_OPER(PUNC_ASSIGN_FLAG, "*=", "*")
        case PUNC_DIV:
            F_OPER(PUNC_ASSIGN_FLAG, "/=", "/")
        case PUNC_MOD:
            F_OPER(PUNC_ASSIGN_FLAG, "%=", "%")
        case PUNC_AND:
            ASS_BIT_OPER("&=", "~&", "&&=", "&&")
        case PUNC_OR:
            ASS_BIT_OPER("|=", "~|", "||=", "||")
        case PUNC_APPEND:
            F_OPER(PUNC_ASSIGN_FLAG, "++=", "++")
            CASE(PUNC_LANGLE, "<")
            CASE(PUNC_RANGLE, ">")
            CASE(PUNC_LS_EQ, "<=")
            CASE(PUNC_GR_EQ, ">=")
            CASE(PUNC_EQ, "==")
            CASE(PUNC_NEQ, "!=")
            CASE(PUNC_COLON, ":")
            CASE(PUNC_LAMBDA, "\\")
        default:
            assert(0);
        }
    }
    else
    {
        switch (tok.val_type)
        {
        case TOK_VAL_STR:
            printf(" `");
            print_str(&tok.val.s);
            puts("`");
            break;
        case TOK_VAL_INT64:
            printf(" `%ld`\n", tok.val.i);
            break;
        case TOK_VAL_FLOAT:
            printf(" `%f`\n", tok.val.f);
            break;
        case TOK_VAL_NONE:
            puts("");
            break;
        default:
            assert(0);
        }
        return;
    }
    // if (s == NULL)
    //     puts("``");
    // else
    //     printf("`%s`\n", s);
}

#undef F_OPER
#undef CASE
#undef ASS_BIT_OPER

void dump_lex(FILE *src_file)
{
    lex_init(NULL, src_file, LEX_UNESCAPE_STR);

    lex_next(LEX_ANY);

    while (lex_tok.lexeme != LEX_END)
    {
        dump_token(lex_tok, lex_tok_pos);
        lex_next(LEX_ANY);
    }
}
