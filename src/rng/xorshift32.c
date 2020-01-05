/*
 * SPDX-License-Identifier: CC0-1.0
 * SPDX-FileCopyrightText: 2020 Lynn Kirby
 */

#include <stdint.h>
#include <stdlib.h>

static uint32_t state;

void xorshift32_init(uint32_t seed)
{
    state = seed;
}

uint32_t xorshift32_next()
{
    state ^= (state << 13);
    state = (state >> 17);
    return state ^= (state << 5);
}
