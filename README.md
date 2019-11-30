# powerbf

A superb brainfuck interpreter.

powerbf is an experiment in making the ultimate brainfuck interpreter, featuring
all the things you'd ever want to make programming in brainfuck less of
a brainfuck. It's a successor to [microbf][microbf], which was my first ever
bytecode VM, and it improves on many things the original failed to do well.

  [microbf]: https://github.com/liquid600pgm/microbf

## Building

To build powerbf, you'll need git and meson. If you have them installed,
execute the following commands in your shell of choice.

```sh
git clone https://github.com/liquid600pgm/powerbf
cd powerbf
meson setup build
ninja -C build
```

It should compile cleanly as C99.

## Running

Run
```
pbf -h
```
to get some help.

## Benchmarks

Benchmarks were conducted using the same method as in microbf (cycle.h), with
computed gotos enabled.
Here are the results:

|         | ticks       | improvement |
| ------- | -----       | ----------- |
| microbf | 20414860928 | —           |
| powerbf | 5942684576  | 3.44x       |

*To perform the benchmark yourself, uncomment `#define BENCHMARK` in*
*`src/cli/pbf_main.c`.*

**But how is it so much better?**

First of all, powerbf does more optimization than microbf. While it does not do
it in the same way (optimizing during compilation in microvf vs after
compilation in powerbf), it can still yield quite an improvement in all
sorts if brainfuck programs, especially badly-written ones.

powerbf does two kinds of optimization:
1. "streak", which folds series of bytecode like `INC 1; INC 1; INC 1` into a
   single opcode (in this case `INC 3`)
2. "balance", which remove any codes that cancel each other out
   (eg. `INC 5; DEC 3` becomes `INC 2`)
It runs these optimizations exactly in the order presented.

Apart from optimizing the bytecode, powerbf also uses a much more efficient
memory representation: instead of using a linked list of memory cells, powerbf
uses a dynamically expanding array of `int8_t`. These two changes are enough
to seriously impact the performance of the VM, as shown by the above benchmark.

The memory change also allows powerbf to run programs that microbf simply wasn't
capable of executing (because it ran out of memory before the program could
finish). An example of such program is [`mandelbrot.b`][mandelbrot.b].

  [mandelbrot.b]: https://github.com/pablojorge/brainfuck/blob/master/programs/mandelbrot.bf