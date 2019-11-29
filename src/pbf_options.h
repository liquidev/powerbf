/* powerbf - a superb brainfuck interpreter
 * copyright (C) iLiquid, 2019 */

#ifndef PBF_OPTIONS_H
#define PBF_OPTIONS_H

#ifndef PBF_COMPUTED_GOTO
# ifndef _MSC_VER
#  define PBF_COMPUTED_GOTO 0
# else
#  define PBF_COMPUTED_GOTO 1
# endif
#endif

#ifndef PBF_MAX_LOOP_DEPTH
# define PBF_MAX_LOOP_DEPTH 256
#endif

#ifndef PBF_STREAM_BUFFER_SIZE
# define PBF_STREAM_BUFFER_SIZE 1024
#endif

#ifndef PBF_VM_TAPE_INIT_SIZE
# define PBF_VM_TAPE_INIT_SIZE 256
#endif

#ifndef PBF_VM_TAPE_GROWTH
# define PBF_VM_TAPE_GROWTH 128
#endif

#endif