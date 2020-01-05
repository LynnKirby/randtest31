<!-- SPDX-License-Identifier: CC0-1.0 -->
<!-- SPDX-FileCopyrightText: 2020 Lynn Kirby -->

# randtest31
> statistical tests of libc and other 31-bit RNGs

**Warning: work in progress.**

This project is for testing *bounded* RNGs like those in C libraries. Instead of
having 32-bit output, they have a 31-bit output from zero to `RAND_MAX`. Using
the output of these RNGs directly or using predefined test suites that assume
32-bit resolution can result in spurious errors. The test harness corrects for
these problems.

Features include:

* A test harness that allows easy testing of random number generators using the
  [PractRand][] and [TestU01][] test suites.

* 31-bit compatible test batteries for TestU01.

* RNGs extracted from various C standard libraries.

* More recent RNGs to compare the libc ones with.

## Building

1. Clone the repository and the submodules:

   ```sh
   git clone https://github.com/LynnKirby/randtest31
   cd randtest31
   git submodule update
   ```

2. Install [TestU01][] into `thirdparty/TestU01`. For example, having extracted
   the source to a temporary directory:

    ```sh
    ./configure --prefix="FULL PATH TO randtest/thirdparty/TestU01"
    make -j
    make -j install
    ```

   There are prebuilt binaries for MinGW and Cygwin on Windows but I have not
   tested them.

3. Build the project with CMake. I recommend installing [Ninja][] and using it
   as the [CMake generator][]:

   ```sh
   mkdir build
   cd build
   cmake .. -GNinja
   ninja
   ```

The executables will be placed under `bin` in your build directory. This
includes the main `randtest31` and PractRand executables like `RNG_test`.

## Usage

Use `randtest31 --help` to see the available RNGs and test modes.

By default, output will be adjusted so that RNG values can be used by the test
suites correctly. You can disable this and use the RNG values directly with the
`-d` flag.

### PractRand

Use the `write` mode along with PractRand's `RNG_test` (which is built
automatically alongside `randtest31`). For example:

```sh
./randtest31 write musl_rand | ./RNG_test stdin
```

### TestU01

A number of test batteries and individual tests have been implemented. Of note
are the ones ending in `31` which are the 31-bit variants. The original versions
of these are also available for comparison. For example:

```sh
./randtest31 rabbit31 musl_rand
# You should see no errors for the 31-bit version.

./randtest31 rabbit musl_rand
# You should see many errors for the uncorrected version.
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

Be aware this project incorporates code from TestU01 which is *not* free
software. See `LICENSES/LicenseRef-testu01.txt` for details.

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
