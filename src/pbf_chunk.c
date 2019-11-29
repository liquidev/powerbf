/* powerbf - a superb brainfuck interpreter
 * copyright (C) iLiquid, 2019 */

#include "pbf_chunk.h"

#include <stdio.h>
#include <stdlib.h>

void pbf_chunk_init(pbf_chunk_t *chunk) {
  chunk->code = (uint32_t *)malloc(PBF_CHUNK_INIT_CAP * sizeof(uint32_t));
  chunk->len = 0;
  chunk->cap = PBF_CHUNK_INIT_CAP;
}

void pbf_chunk_deinit(pbf_chunk_t *chunk) {
  free(chunk->code);
}

void pbf_chunk__alloc_more(pbf_chunk_t *chunk) {
  chunk->cap += PBF_CHUNK_REALLOC_INCREMENT;
  chunk->code = realloc(chunk->code, chunk->cap * sizeof(uint32_t));
}

size_t pbf_chunk_write_32(pbf_chunk_t *chunk, uint32_t code) {
  if (chunk->len + 1 > chunk->cap) {
    pbf_chunk__alloc_more(chunk);
  }
  size_t result = chunk->len;
  chunk->code[chunk->len] = code;
  ++chunk->len;
  return result;
}

size_t pbf_chunk_write_op(pbf_chunk_t *chunk, pbf_opcode_t op, uint32_t rest) {
  return pbf_chunk_write_32(chunk, (op << 28) | (rest & 0x0FFFFFFF));
}

void pbf_chunk_patch_opcode(pbf_chunk_t *chunk,
                            size_t offset, pbf_opcode_t op) {
  chunk->code[offset] &= 0x0FFFFFFF;
  chunk->code[offset] |= (op << 28);
}

void pbf_chunk_patch_operand(pbf_chunk_t *chunk,
                             size_t offset, uint32_t operand) {
  chunk->code[offset] &= 0xF0000000;
  chunk->code[offset] |= (operand & 0x0FFFFFFF);
}

uint32_t pbf_chunk_erase(pbf_chunk_t *chunk, size_t offset) {
  uint32_t result = chunk->code[offset];
  for (size_t i = offset + 1; i < chunk->len; ++i) {
    chunk->code[i - 1] = chunk->code[i];
  }
  --chunk->len;
  for (size_t i = 0; i < chunk->len; ++i) {
    pbf_opcode_t opcode = chunk->code[i] >> 28;
    if (opcode == PBF_OP_JZE || opcode == PBF_OP_JNZ) {
      uint32_t operand = chunk->code[i] & 0x0FFFFFFF;
      if (operand >= offset) {
        pbf_chunk_patch_operand(chunk, i, operand - 1);
      }
    }
  }
  return result;
}

void pbf_chunk_disassemble(pbf_chunk_t *chunk) {
  for (size_t i = 0; i < chunk->len; ++i) {
    uint32_t code = chunk->code[i];
    pbf_opcode_t opcode = code >> 28;
    uint32_t operand = code & 0x0FFFFFFF;

    /* powerbf uses a 28-bit address space, 4 bits are used for opcodes,
       hence we print 7 hex digits instead of 8 */
    fprintf(stderr, "%07X ", (unsigned int)i);
    switch (opcode) {
    case PBF_OP_INC: { fprintf(stderr, "INC %i", operand); } break;
    case PBF_OP_DEC: { fprintf(stderr, "DEC %i", operand); } break;
    case PBF_OP_PIN: { fprintf(stderr, "PIN %i", operand); } break;
    case PBF_OP_PDE: { fprintf(stderr, "PDE %i", operand); } break;
    case PBF_OP_PUT: { fprintf(stderr, "PUT %i", operand); } break;
    case PBF_OP_GET: { fprintf(stderr, "GET %i", operand); } break;
    case PBF_OP_JZE: { fprintf(stderr, "JZE $%07X", operand); } break;
    case PBF_OP_JNZ: { fprintf(stderr, "JNZ $%07X", operand); } break;
    case PBF_OP_RES0:
    case PBF_OP_RES1:
    case PBF_OP_RES2:
    case PBF_OP_RES3:
    case PBF_OP_RES4:
    case PBF_OP_RES5:
    case PBF_OP_RES6: { fprintf(stderr, "<reserved opcode>"); } break;
    case PBF_OP_HLT: { fprintf(stderr, "HLT"); }
    }
    fprintf(stderr, "\n");
  }
}
