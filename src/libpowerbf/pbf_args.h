/* powerbf - a superb brainfuck interpreter
 * copyright (C) iLiquid, 2019 */

#ifndef PBF_ARGS_H
#define PBF_ARGS_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

struct pbf_argp;

typedef void (*pbf_argp_handler_proc)(struct pbf_argp *, char *);

typedef struct pbf_arg_handler {
  pbf_argp_handler_proc impl;
  bool flag;
} pbf_arg_handler_t;

typedef struct pbf_argp {
  char *executable;

  char default_arg;
  pbf_arg_handler_t handlers[128];

  void *userdata;
} pbf_argp_t;

void pbf_argp_init(pbf_argp_t *p);

void pbf_argp_parse(pbf_argp_t *p, int argc, char *argv[]);

void pbf_argp_on_flag(pbf_argp_t *p, bool flag, char c,
                      pbf_argp_handler_proc impl);
void pbf_argp_on_argument(pbf_argp_t *p, pbf_argp_handler_proc impl);

#endif
