#ifndef YA_STR_H
#define YA_STR_H

#include <stdio.h>
#include "vec.h"

typedef vec_t string_t;

#define string_new() vec_new()
#define string_push(str, c) vec_append(str, &(c), sizeof(char))
#define string_append(str, c_str, len) vec_append(str, c_str, (len) * sizeof(char))

#define STRING_DATA(str) VEC_DATA(str, char)

/* Print to file with given lenght, zeros also printed. */
void string_fprint(FILE *f, string_t *str);
string_t *string_escape(string_t *str);

#endif