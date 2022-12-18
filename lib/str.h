#ifndef YA_STR_H
#define YA_STR_H

#include <stdio.h>
#include "vec.h"

typedef vec_t string_t;

/* -> string_t * */
#define string_new() vec_new()
/* string_t *str, char c -> string_t *str
   c should be lvalue. */
#define string_push(str, c) vec_append(str, &(c), sizeof(char))
/* string_t *str, const char *c_str, size_t len -> string_t * */
#define string_append(str, c_str, len) vec_append(str, c_str, (len) * sizeof(char))
/* const string_t *str1, const string_t *str2 -> bool */
#define string_equals(str1, str2) vec_equals(str1, str2)

/* const string_t *str -> char * */
#define STRING_DATA(str) VEC_DATA(str, char)

/* Print to file with given lenght, zeros also printed. */
void string_fprint(FILE *f, string_t *str);
string_t *string_escape(string_t *str);

#endif