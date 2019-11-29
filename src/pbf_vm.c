/* powerbf - a superb brainfuck interpreter
 * copyright (C) iLiquid, 2019 */

#include <string.h>

#include "pbf_options.h"
#include "pbf_vm.h"

void pbf_vm_init(pbf_vm_t *vm) {
  vm->tape = (int8_t *)malloc(PBF_VM_TAPE_INIT_SIZE * sizeof(int8_t));
  vm->tape_len = PBF_VM_TAPE_INIT_SIZE;
  vm->put_impl = NULL;
  vm->get_impl = NULL;
}

void pbf_vm_deinit(pbf_vm_t *vm) {
  free(vm->tape);
}

void pbf_vm__reset_tape(pbf_vm_t *vm) {
  for (size_t i = 0; i < vm->tape_len; ++i) {
    vm->tape[i] = 0;
  }
  vm->tape_pointer = vm->tape_len / 2;
}

void pbf_vm__tape_expand_forward(pbf_vm_t *vm) {
  vm->tape_len += PBF_VM_TAPE_GROWTH;
  vm->tape = realloc(vm->tape, vm->tape_len * sizeof(int8_t));
}

void pbf_vm__tape_expand_backward(pbf_vm_t *vm) {
  size_t old_tape_len = vm->tape_len;
  vm->tape_len += PBF_VM_TAPE_GROWTH;
  vm->tape = realloc(vm->tape, vm->tape_len * sizeof(int8_t));
  memmove(&vm->tape[PBF_VM_TAPE_GROWTH], vm->tape, old_tape_len);
  memset(vm->tape, 0, PBF_VM_TAPE_GROWTH);
  vm->tape_pointer += PBF_VM_TAPE_GROWTH;
}

void pbf_vm_run(pbf_vm_t *vm, pbf_chunk_t *chunk) {
  size_t pc = 0;

  pbf_vm__reset_tape(vm);

  for (;;) {
    uint32_t operand;

    #ifdef PBF_COMPUTED_GOTO
    # define ccase(opcode) case_##opcode: {
    # define dispatch()    operand = chunk->code[pc] & 0x0FFFFFFF; \
                           goto *dispatch_table[chunk->code[pc++] >> 28]
    # define end_case      } dispatch()
    static void *dispatch_table[] = {
      &&case_PBF_OP_INC, &&case_PBF_OP_DEC,
      &&case_PBF_OP_PIN, &&case_PBF_OP_PDE,
      &&case_PBF_OP_PUT, &&case_PBF_OP_GET,
      &&case_PBF_OP_JZE, &&case_PBF_OP_JNZ,
      &&case_PBF_OP_RES0, &&case_PBF_OP_RES1,
      &&case_PBF_OP_RES2, &&case_PBF_OP_RES3,
      &&case_PBF_OP_RES4, &&case_PBF_OP_RES5,
      &&case_PBF_OP_RES6,
      &&case_PBF_OP_HLT
    };
    #else
    # define ccase(opcode) case opcode: {
    # define dispatch()
    # define end_case      } break
    #endif

    #ifndef PBF_COMPUTED_GOTO
    switch (operand = chunk->code[pc], (chunk->code[pc++] >> 28)) {
    #else
    dispatch();
    #endif

    /* cell operations */
    ccase(PBF_OP_INC)
      vm->tape[vm->tape_pointer] += operand;
    end_case;
    ccase(PBF_OP_DEC)
      vm->tape[vm->tape_pointer] -= operand;
    end_case;
    /* pointer operations */
    ccase(PBF_OP_PIN)
      vm->tape_pointer += operand;
      if (vm->tape_pointer >= (int64_t)vm->tape_len) {
        pbf_vm__tape_expand_forward(vm);
      }
    end_case;
    ccase(PBF_OP_PDE)
      vm->tape_pointer -= operand;
      if (vm->tape_pointer < 0) {
        pbf_vm__tape_expand_backward(vm);
      }
    end_case;
    /* IO */
    ccase(PBF_OP_PUT)
      if (vm->put_impl != NULL) {
        for (uint32_t i = 0; i < operand; ++i) {
          vm->put_impl(vm, (char)vm->tape[vm->tape_pointer]);
        }
      }
    end_case;
    ccase(PBF_OP_GET)
      if (vm->get_impl != NULL) {
        for (uint32_t i = 0; i < operand; ++i) {
          vm->tape[vm->tape_pointer] = (int8_t)vm->get_impl(vm);
        }
      }
    end_case;
    /* flow control */
    ccase(PBF_OP_JZE)
      if (vm->tape[vm->tape_pointer] == 0) {
        pc = operand;
      }
    end_case;
    ccase(PBF_OP_JNZ)
      if (vm->tape[vm->tape_pointer] != 0) {
        pc = operand;
      }
    end_case;
    /* reserved */
    ccase(PBF_OP_RES0) end_case;
    ccase(PBF_OP_RES1) end_case;
    ccase(PBF_OP_RES2) end_case;
    ccase(PBF_OP_RES3) end_case;
    ccase(PBF_OP_RES4) end_case;
    ccase(PBF_OP_RES5) end_case;
    ccase(PBF_OP_RES6) end_case;
    /* halt */
    ccase(PBF_OP_HLT)
      break;
    end_case;

    #ifndef PBF_COMPUTED_GOTO
    }
    #endif

    #undef end_case
    #undef dispatch
    #undef ccase
  }
}
