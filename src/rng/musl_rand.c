/*
 * SPDX-License-Identifier: LicenseRef-MIT-musl
 * SPDX-FileCopyrightText: 2013 Rich Felker
 * SPDX-FileCopyrightText: 2020 Lynn Kirby
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t seed;

void musl_rand_init(uint32_t s)
{
	seed = s-1;
}

uint32_t musl_rand_next()
{
	seed = 6364136223846793005ULL*seed + 1;
	return seed>>33;
}
