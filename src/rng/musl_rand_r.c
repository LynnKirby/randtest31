/*
 * SPDX-License-Identifier: LicenseRef-MIT-musl
 * SPDX-FileCopyrightText: 2013 Rich Felker
 * SPDX-FileCopyrightText: 2020 Lynn Kirby
 *
 * Modified from musl src/prng/rand_r.c.
 */

#include <stdint.h>

static unsigned temper(unsigned x)
{
	x ^= x>>11;
	x ^= x<<7 & 0x9D2C5680;
	x ^= x<<15 & 0xEFC60000;
	x ^= x>>18;
	return x;
}

static unsigned state;

void musl_rand_r_init(uint32_t seed)
{
    state = seed;
}

uint32_t musl_rand_r_next()
{
	return temper(state = state * 1103515245 + 12345)/2;
}

