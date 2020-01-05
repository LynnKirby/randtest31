/*
 * SPDX-License-Identifier: CC0-1.0
 * SPDX-FileCopyrightText: 2020 Lynn Kirby
 */

#include <stdint.h>
#include <limits.h>
#include "pcg_variants.h"

static pcg32f_random_t state;

void pcg32f_init(uint32_t seed)
{
    pcg32f_srandom_r(&state, seed);
}

uint32_t pcg32f_next()
{
    return pcg32f_random_r(&state);
}
