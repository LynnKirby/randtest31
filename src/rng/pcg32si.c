/*
 * SPDX-License-Identifier: CC0-1.0
 * SPDX-FileCopyrightText: 2020 Lynn Kirby
 */

#include <stdint.h>
#include <limits.h>
#include "pcg.h"

static pcg32si_random_t state;

void pcg32si_init(uint32_t seed)
{
    pcg32si_srandom_r(&state, seed);
}

uint32_t pcg32si_next()
{
    return pcg32si_random_r(&state);
}
