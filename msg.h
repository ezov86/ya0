#ifndef YA_MSG_H
#define YA_MSG_H

/* Should be < 512 bytes, because allocation is done on the stack before copying to the heap. */
#define MSG_MAX_SIZE 256
/* Be carefull with `%s` in format string. If there's more than one of those in fmt, then use smaller values. */
#define MSG_INSERTED_STR_MAX_SIZE 64

typedef enum {
    YA_LIB_ALLOC_FATAL_ERR,
    
    /* %c */
    YA_COMP_LEX_INVALID_CHAR,
    /* %c */
    YA_COMP_LEX_INVALID_ESC,
    /* %s */
    YA_COMP_LEX_INVALID_INT,
    /* %s */
    YA_COMP_LEX_TOO_BIG_INT,
    /* %s */
    YA_COMP_LEX_TOO_SMALL_INT,
    YA_COMP_LEX_UNEXPECTED_EOL,
    YA_COMP_LEX_UNEXPECTED_EOF,
} msg_id_t;

extern char *msgs[];

#endif