#ifndef YA_MSG_H
#define YA_MSG_H

typedef enum {
    YA_LIB_ALLOC_FATAL_ERR,
    
    /* %c */
    YA_COMP_LEX_INVALID_CHAR,
    /* %c */
    YA_COMP_LEX_INVALID_ESC,
    /* %s */
    YA_COMP_LEX_INVALID_ESC_HEX,
    /* %s */
    YA_COMP_LEX_INVALID_INT,
    /* %s */
    YA_COMP_LEX_TOO_BIG_INT,
    /* %s */
    YA_COMP_LEX_TOO_SMALL_INT,
} msg_code_t;

extern char *msgs[];

#endif