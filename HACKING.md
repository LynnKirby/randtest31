<!-- SPDX-License-Identifier: CC0-1.0 -->
<!-- SPDX-FileCopyrightText: 2020 Lynn Kirby -->

# Development Notes

## Adding a new RNG

1. Create a new file `src/rng/my_rng.c`. Write your implementation there. It
   should have a `void my_rng_init(uint32_t seed)` initialization/seeding
   function and a `uint32 my_rng_next()` output function. Everything is
   currently in global variables right now (sadly).

2. Add your source file to `src/CMakeLists.txt`.

3. Add your RNG identifier and description to `src/rngs.def`.