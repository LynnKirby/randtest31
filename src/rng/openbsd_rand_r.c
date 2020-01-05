/*
 * SPDX-License-Identifier: CC0-1.0
 * SPDX-FileCopyrightText: 2020 Lynn Kirby
 */

#include <stdlib.h>
#include <stdint.h>

unsigned state;

void openbsd_rand_r_init(uint32_t seed)
{
    state = seed;
}

uint32_t openbsd_rand_r_next()
{
	state = state * 1103515245 + 12345;
	return (state & RAND_MAX);
}
