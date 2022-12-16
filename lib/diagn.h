#ifndef YA_ERROR_H
#define YA_ERROR_H

#include <stdint.h>
#include <stdio.h>
#include "msg.h"
#include "collections.h"

typedef enum
{
    LOG_ERR,
    LOG_WARN,
    LOG_INFO,
    LOG_VERBOSE_INFO
} severity_t;

typedef struct
{
    uint16_t line;
    uint16_t col;
} pos_t;

typedef struct {
    severity_t severity;
    char *filename;
    pos_t pos;
    msg_code_t code;
    char *formatted_msg;
} diagn_info_t;

typedef struct {
    vec_t vec;
    FILE *file;
} diagn_bag_t;

void diagn_add(diagn_bag_t *bag, diagn_info_t info);
void diagn_flush_to_file(diagn_bag_t *bag);

#endif