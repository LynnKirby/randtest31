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

Clone the repository then build the project with CMake. I recommend installing
[Ninja][] and using it as the [CMake generator][].

```sh
git clone --recurse-submodules https://github.com/LynnKirby/randtest31
cd randtest31
mkdir build
cd build
cmake -GNinja ..
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

**Warning:** You cannot use PowerShell with this pipe command. PowerShell pipes text, not
binary data, between processes which breaks everything.

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

## License

All original code is released to the public domain under Creative Commons Zero.
There is a significant quantity of modified third party code under various
licenses. Check the license identifiers in header comments to see what license
that file uses. The licenses themselves are in the `LICENSES` subdirectory.

Be aware this project incorporates code from TestU01 which is *not* free
software. See `LICENSES/LicenseRef-testu01.txt` for details.

## Links

* Lynn Kirby's [PractRand fork][] and [TestU01 fork][]. They're used as git
  submodules in this project to easily build everything with CMake.

* Lemire's [testingRNG][]. A project with similar goals to this one but a much
  different implementation.

[PractRand]: http://pracrand.sourceforge.net/
[TestU01]: http://simul.iro.umontreal.ca/testu01/tu01.html
[Ninja]: https://ninja-build.org/
[CMake generator]: https://cmake.org/cmake/help/latest/generator/Ninja.html
[PractRand fork]: https://github.com/LynnKirby/PractRand
[TestU01 fork]: https://github.com/LynnKirby/TestU01
[testingRNG]: https://github.com/lemire/testingRNG
