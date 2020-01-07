/*
 * SPDX-License-Identifier: CC0-1.0
 * SPDX-FileCopyrightText: 2020 Lynn Kirby
 */

#include <stdint.h>
#include <limits.h>
#include "pcg.h"

static pcg32s_random_t state;

void pcg32_init(uint32_t seed)
{
    pcg32s_srandom_r(&state, seed);
}

uint32_t pcg32_next()
{
    return pcg32s_random_r(&state);
}
