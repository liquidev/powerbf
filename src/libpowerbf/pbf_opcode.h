/* powerbf - a superb brainfuck interpreter
 * copyright (C) iLiquid, 2019 */

#ifndef PBF_OPCODE_H
#define PBF_OPCODE_H

typedef enum pbf_opcode {
  /* cell operations */
  PBF_OP_INC = 000, /* increment */
  PBF_OP_DEC = 001, /* decrement */
  /* pointer operations */
  PBF_OP_PIN = 002, /* pointer increment */
  PBF_OP_PDE = 003, /* pointer decrement */
  /* IO */
  PBF_OP_PUT = 004, /* put char */
  PBF_OP_GET = 005, /* get char */
  /* flow control */
  PBF_OP_JZE = 006, /* jump if zero */
  PBF_OP_JNZ = 007, /* jump if not zero */
  /* reserved */
  PBF_OP_RES0 = 010, PBF_OP_RES1 = 011, PBF_OP_RES2 = 012, PBF_OP_RES3 = 013,
  PBF_OP_RES4 = 014, PBF_OP_RES5 = 015, PBF_OP_RES6 = 016,
  /* halt */
  PBF_OP_HLT = 017
} pbf_opcode_t;

#endif
