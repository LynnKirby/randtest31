/*
 * SPDX-License-Identifier: CC0-1.0
 * SPDX-FileCopyrightText: 2020 Lynn Kirby
 */

#include <stdint.h>
#include <limits.h>

unsigned state;

void bsd_rand_r_init(uint32_t seed)
{
    state = seed;
}

uint32_t bsd_rand_r_next()
{
    state = state * 1103515245 + 12345;
	  return (state & INT32_MAX);
}
