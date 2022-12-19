#ifndef YA_PLATFORM_H
#define YA_PLATFORM_H

#include <stdint.h>
#include <stddef.h>

/* Some platform dependent types and macros defined here. */

/* All defined here is for 64-bit system.
  TODO: add support to 32-bits and embedded systems. */

typedef uint32_t pl_int_t;
typedef float pl_real_t;

typedef uint16_t pl_blk_size_t;

#endif