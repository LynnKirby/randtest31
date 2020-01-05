/*
 * SPDX-License-Identifier: LicenseRef-newlib
 * SPDX-FileCopyrightText: 2020 Lynn Kirby
 *
 * Modified from newlib newlib/libc/stdlib/rand.c
 */

#include <stdint.h>
#include <stdlib.h>

uint64_t state;

void newlib_rand_init(uint32_t seed)
{
    state = seed;
}

uint32_t newlib_rand_next()
{
    /* This multiplier was obtained from Knuth, D.E., "The Art of
       Computer Programming," Vol 2, Seminumerical Algorithms, Third
       Edition, Addison-Wesley, 1998, p. 106 (line 26) & p. 108 */
    state = state * 6364136223846793005LL + 1;
    return (state >> 32) & RAND_MAX;
}
