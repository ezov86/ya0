#ifndef LOG_H
#define LOG_H

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "lib/str.h"
#include "msg.h"

typedef enum
{
    LOG_FATAL,
    LOG_ERR,
    LOG_WARN,
    LOG_INFO,
    LOG_VERBOSE_INFO
} log_severity_t;

typedef struct
{
    uint16_t line;
    uint16_t col;
} pos_t;

typedef struct
{
    log_severity_t severity;
    pos_t pos;
    msg_id_t id;
    /* If no file was given, saving formatted message here, else NULL. */
    char *str;
} log_msg_t;

typedef struct
{
    string_t *file_path;
    vec_t *messages;
    FILE *file;
} log_bag_t;

#define LOG_NO_POS ((pos_t){.line = 0, .col = 0})

/* Vector of log_msg_t. */
extern vec_t *log_messages;
/* Path to compiling file. */
extern string_t *log_file_path;

void log_init(string_t *file_path, FILE *file);
/* Any string that will be inserted with formatting should be prepared with log_prepare_str(). */
void log_add_msg(log_severity_t severity, pos_t pos, msg_id_t id, ...);
/* Prepare string_t before inserting by formatting into logging message: escape non-ASCII chars and cut.
   Use MSG_INSERTED_STR_MAX_SIZE as size for messages with one inserted string. */
string_t *log_prepare_str(string_t *str);

#endif