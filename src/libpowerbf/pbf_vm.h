/* powerbf - a superb brainfuck interpreter
 * copyright (C) iLiquid, 2019 */

#ifndef PBF_VM_H
#define PBF_VM_H

#include "pbf_chunk.h"

struct pbf_vm;

typedef struct pbf_vm {
  /* options */
  size_t max_tape_size;
  void (*put_impl)(struct pbf_vm *, char);
  char (*get_impl)(struct pbf_vm *);
  void *userdata;

  /* tape */
  int8_t *tape;
  size_t tape_len;
  int64_t tape_pointer;
} pbf_vm_t;

void pbf_vm_init(pbf_vm_t *vm);
void pbf_vm_deinit(pbf_vm_t *vm);

void pbf_vm_run(pbf_vm_t *vm, pbf_chunk_t *chunk);

#endif
