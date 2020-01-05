/*
 * SPDX-License-Identifier: CC0-1.0
 * SPDX-FileCopyrightText: 2020 Lynn Kirby
 */

#include <stdint.h>

uint32_t state;

void splitmix32_init(uint32_t seed)
{
    state = seed;
}

uint32_t splitmix32_next()
{
    uint32_t z = state += 0x9e3779b9;
    z ^= z >> 15; // 16 for murmur3
    z *= 0x85ebca6b;
    z ^= z >> 13;
    z *= 0xc2b2ae35;
    return z ^= z >> 16;
}
