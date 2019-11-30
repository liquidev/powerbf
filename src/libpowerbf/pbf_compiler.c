/* powerbf - a superb brainfuck interpreter
 * copyright (C) iLiquid, 2019 */

#include "pbf_compiler.h"
#include "pbf_panic.h"
#include "pbf_options.h"

void pbf_compile(pbf_chunk_t *c, pbf_instream_t *in) {
  /* reading from stream */
  char read_buffer[PBF_STREAM_BUFFER_SIZE];
  size_t read_bytes;

  /* loops */
  uint32_t loop_starts[PBF_MAX_LOOP_DEPTH];
  int loop_starts_len = 0;

  while (true) {
    read_bytes = in->read_impl(in, read_buffer, sizeof(read_buffer));
    for (int i = 0; i < (int)read_bytes; ++i) {
      char opc = read_buffer[i];

      #define primitive(ch, op) \
        case ch: { \
          pbf_chunk_write_op(c, op, 1); \
        } break;

      switch (opc) {
      primitive('+', PBF_OP_INC)
      primitive('-', PBF_OP_DEC)
      primitive('>', PBF_OP_PIN)
      primitive('<', PBF_OP_PDE)
      primitive('.', PBF_OP_PUT)
      primitive(',', PBF_OP_GET)
      case '[': {
        uint32_t hole = pbf_chunk_write_op(c, PBF_OP_JZE, 0);
        loop_starts[loop_starts_len] = hole;
        ++loop_starts_len;
      } break;
      case ']': {
        uint32_t start = loop_starts[loop_starts_len - 1];
        pbf_chunk_write_op(c, PBF_OP_JNZ, start);
        pbf_chunk_patch_operand(c, start, c->len);
        --loop_starts_len;
      } break;
      }
    }
    if (read_bytes < sizeof(read_buffer)) {
      break;
    }

    #undef primitive
  }
  if (loop_starts_len > 0) {
    pbf_panic("unmatched loop brackets");
  }
  if (loop_starts_len < 0) {
    pbf_panicf("internal error: negative loop nesting"
               "(loop_starts_len = %i)\nplease report this!", loop_starts_len);
  }

  pbf_chunk_write_op(c, PBF_OP_HLT, 0);
}

void pbf_optimize(pbf_chunk_t *chunk) {
  /* optimize */
  while (true) {
    size_t del_begin, del_len;
    for (size_t i = 0; i < chunk->len; ++i) {
      pbf_opcode_t opcode = chunk->code[i] >> 28;
      uint32_t operand = chunk->code[i] & 0x0FFFFFFFF;

      /* 'streak' optimization
         optimizes streaks of opcodes, like ++++++, ---------
         applied for: +-<>., */
      bool streak = false;
      if (opcode >= PBF_OP_INC && opcode <= PBF_OP_GET) {
        for (size_t j = i + 1; j < chunk->len; ++j) {
          pbf_opcode_t other_opcode = chunk->code[j] >> 28;
          if (other_opcode == opcode) {
            if (!streak) {
              del_begin = j;
              del_len = 0;
            }
            uint32_t other_operand = chunk->code[j] & 0x0FFFFFFF;
            streak = true;
            operand += other_operand;
            ++del_len;
          } else {
            break;
          }
        }
      }
      if (streak) {
        pbf_chunk_patch_operand(chunk, i, operand);
        goto opt_done;
      }

      /* 'balance' optimization
         optimizes opcodes that cancel each other out,
         eg. +++---- becomes -
         applied for: +-<> */
      bool balance = false;
      switch (opcode) {
      #define balance_for(fst, snd) \
        case fst: { \
          pbf_opcode_t next_opcode = chunk->code[i + 1] >> 28; \
          /* ↑ this is safe, because HLT is not optimized, therefore
             there's always an opcode after the current one */ \
          if (next_opcode == snd) { \
            uint32_t next_operand = chunk->code[i + 1] & 0x0FFFFFFF; \
            int diff = (int)operand - (int)next_operand; \
            if (diff == 0) { \
              del_begin = i; \
              del_len = 2; \
            } else if (diff > 0) { \
              pbf_chunk_patch_operand(chunk, i, diff); \
              del_begin = i + 1; \
              del_len = 1; \
            } else if (diff < 0) { \
              pbf_chunk_patch_operand(chunk, i + 1, -diff); \
              del_begin = i; \
              del_len = 1; \
            } \
            balance = true; \
          } \
        } break;
      balance_for(PBF_OP_INC, PBF_OP_DEC)
      balance_for(PBF_OP_DEC, PBF_OP_INC)
      balance_for(PBF_OP_PIN, PBF_OP_PDE)
      balance_for(PBF_OP_PDE, PBF_OP_PIN)
      #undef balance_for
      default: {}
      }
      if (balance) {
        goto opt_done;
      }
    }
    break;
  opt_done:
    /* delete marked section */
    pbf_chunk_erase(chunk, del_begin, del_len);
    continue;
  }
}

#undef DELETE
