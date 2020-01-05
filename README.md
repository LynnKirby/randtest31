<!-- SPDX-License-Identifier: CC0-1.0 -->
<!-- SPDX-FileCopyrightText: 2020 Lynn Kirby -->

# randtest31
> statistical tests of libc and other 31-bit RNGs

**Warning: work in progress.**

This project contains:

* a test harness that allows easy testing of random number generators using the
  [PractRand][] and [TestU01][] test suites
* RNGs extracted from various C standard libraries
* more recent RNGs to compare the libc ones with

It's purpose is to compare *bounded* RNGs like those in C libraries. Instead of
having 32-bit output, they have a 31-bit output from zero to `RAND_MAX`. Naively
using the output of these functions leads to false errors in test suites. The
test harness corrects for this.

## Building

1. Clone the repository and the submodules:

   ```
   git clone https://github.com/LynnKirby/randtest31
   cd randtest31
   git submodule update
   ```

2. Install [TestU01][] into `thirdparty/TestU01`. For example, having extracted
   the source to a temporary directory:

    ```
    ./configure --prefix="FULL PATH TO randtest/thirdparty/TestU01"
    make -j
    make -j install
    ```

   There are prebuilt binaries for MinGW and Cygwin on Windows but I have not
   tested them.

3. Build the project with CMake. I recommend installing [Ninja][] and using it
   as the [CMake generator][]:

   ```
   mkdir build
   cd build
   cmake .. -GNinja
   ninja
   ```

The executables will be placed under `bin` in your build directory. This
includes the main `randtest31` and PractRand executables like `RNG_test`.

## Running

`randtest31` has two modes. Use `randtest31 --help` to see available RNGs.

**`practrand`** writes to standard output and is intended to be consumed by
PractRand's `RNG_test` (though it could also be used to generate data to a file,
or piped to a different program). For example:

```
./randtest31 practrand musl_rand | ./RNG_test stdin
```

**`smallcrush`** runs the TestU01 SmallCrush test suite. For example:

```
./randtest31 smallcrush musl_rand
```

## Adding a new RNG

1. Create a new file `src/rng/my_rng.c`. Write your implementation there. It
   should have a `void my_rng_init(uint32_t seed)` initialization/seeding
   function and a `uint32 my_rng_next()` output function. Everything is
   currently in global variables right now (sadly).

2. Add your source file to `src/CMakeLists.txt`.

3. Add your RNG identifier and description to `src/rngs.def`.

## License

All original code is released to the public domain under Creative Commons Zero.
There is a significant quantity of modified third party code under various
licenses. Check the license identifiers in header comments to see what license
that file uses. The licenses themselves are in the `LICENSES` subdirectory.

## Links

* Lynn Kirby's [PractRand fork][]. It's used as a git submodule in this project
  to easily build and use everything with CMake.

* Lemire's [testingRNG][]. A project with similar goals to this one but a much
  different implementation.

[PractRand]: http://pracrand.sourceforge.net/
[TestU01]: http://simul.iro.umontreal.ca/testu01/tu01.html
[Ninja]: https://ninja-build.org/
[CMake generator]: https://cmake.org/cmake/help/latest/generator/Ninja.html
[PractRand fork]: https://github.com/LynnKirby/PractRand
[testingRNG]: https://github.com/lemire/testingRNG
