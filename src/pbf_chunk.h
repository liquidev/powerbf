/* powerbf - a superb brainfuck interpreter
 * copyright (C) iLiquid, 2019 */

#ifndef PBF_CHUNK_H
#define PBF_CHUNK_H

#include <stdlib.h>
#include <stdint.h>

#include "pbf_opcode.h"

#define PBF_CHUNK_INIT_CAP          128
#define PBF_CHUNK_REALLOC_INCREMENT 128

typedef struct pbf_chunk {
  uint32_t *code;
  size_t len, cap;
} pbf_chunk_t;

void pbf_chunk_init(pbf_chunk_t *chunk);
void pbf_chunk_deinit(pbf_chunk_t *chunk);

size_t pbf_chunk_write_32(pbf_chunk_t *chunk, uint32_t code);
size_t pbf_chunk_write_op(pbf_chunk_t *chunk, pbf_opcode_t op, uint32_t rest);
uint32_t pbf_chunk_erase(pbf_chunk_t *chunk, size_t offset);

void pbf_chunk_patch_opcode(pbf_chunk_t *chunk, size_t offset, pbf_opcode_t op);
void pbf_chunk_patch_operand(pbf_chunk_t *chunk,
                             size_t offset, uint32_t operand);

void pbf_chunk_disassemble(pbf_chunk_t *chunk);

#endif
