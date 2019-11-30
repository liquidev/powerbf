/* powerbf - a superb brainfuck interpreter
 * copyright (C) iLiquid, 2019 */

#ifndef PBF_PANIC_H
#define PBF_PANIC_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define pbf_panicf(message, ...) \
  do { \
    fprintf(stderr, message, __VA_ARGS__); \
    exit(1); \
  } while (false);

#define pbf_panic(message) \
  pbf_panicf("%s", message)

#endif
