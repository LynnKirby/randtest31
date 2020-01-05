# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: 2020 Lynn Kirby

add_executable(
    randtest31
    main.c
    rng/glibc_rand_r.c
    rng/musl_rand_r.c
    rng/newlib_rand_r.c
    rng/pcg32si.c
    rng/pcg32f.c
    rng/musl_rand.c
    rng/newlib_rand.c
    rng/netbsd_rand.c
    rng/freebsd_rand.c
    rng/freebsd_random.c
    rng/openbsd_rand_r.c
    rng/xorshift32.c
    rng/mulberry32.c
    rng/splitmix32.c
    rng/musl_random.c
    rng/glibc_random.c
)

target_link_libraries(randtest31 PRIVATE LittleIntPacker pcg TestU01)
