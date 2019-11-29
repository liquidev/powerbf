/* powerbf - a superb brainfuck interpreter
 * copyright (C) iLiquid, 2019 */

#include "pbf_args.h"
#include "pbf_panic.h"

void pbf_argp_init(pbf_argp_t *p) {
  p->default_arg = ' ';
  for (int i = 0; i < 128; ++i) {
    p->handlers[i].impl = NULL;
    p->handlers[i].flag = true;
  }
  p->userdata = NULL;
}

void pbf_argp_parse(pbf_argp_t *p, int argc, char *argv[]) {
  p->executable = argv[0];
  pbf_arg_handler_t current_handler;
  bool get_flag = true;
  for (int i = 1; i < argc; ++i) {
    char *arg = argv[i];
    if (get_flag) {
      if (strlen(arg) == 2 && arg[0] == '-') {
        if (p->handlers[(int)arg[1]].impl != NULL) {
          current_handler = p->handlers[(int)arg[1]];
          if (current_handler.flag) {
            current_handler.impl(p, NULL);
          } else {
            get_flag = false;
          }
        } else {
          pbf_panicf("unknown argument: -%c\n", arg[1]);
        }
      } else {
        if (p->handlers['\x00'].impl != NULL) {
          p->handlers['\x00'].impl(p, arg);
        }
      }
    } else {
      current_handler.impl(p, arg);
      get_flag = true;
    }
  }
}

void pbf_argp_on_flag(pbf_argp_t *p, bool flag, char c,
                      pbf_argp_handler_proc impl) {
  p->handlers[(int)c].impl = impl;
  p->handlers[(int)c].flag = flag;
}

void pbf_argp_on_argument(pbf_argp_t *p, pbf_argp_handler_proc impl) {
  pbf_argp_on_flag(p, true, '\x00', impl);
}
