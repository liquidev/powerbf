/* powerbf - a superb brainfuck interpreter
 * copyright (C) iLiquid, 2019 */

#ifndef PBF_COMPILER_H
#define PBF_COMPILER_H

#include <stdbool.h>
#include <stdlib.h>

#include "pbf_chunk.h"

struct pbf_instream;

typedef struct pbf_instream {
  size_t (*read_impl)(struct pbf_instream *, char *, size_t);
  void *userdata;
} pbf_instream_t;

void pbf_compile(pbf_chunk_t *c, pbf_instream_t *in);

void pbf_optimize(pbf_chunk_t *c);

#endif
