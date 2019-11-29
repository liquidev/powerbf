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
