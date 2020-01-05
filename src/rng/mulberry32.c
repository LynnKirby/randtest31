/*
 * SPDX-License-Identifier: CC0-1.0
 * SPDX-FileCopyrightText: 2017 by Tommy Ettinger (tommy.ettinger@gmail.com)
 * SPDX-FileCopyrightText: 2020 Lynn Kirby
 *
 * Modified from https://gist.github.com/tommyettinger/46a874533244883189143505d203312c
 */

#include <stdint.h>
#include <stdlib.h>

uint32_t x;

void mulberry32_init(uint32_t seed)
{
    x = seed;
}

uint32_t mulberry32_next()
{
  uint32_t z = (x += 0x6D2B79F5UL);
  z = (z ^ (z >> 15)) * (z | 1UL);
  z ^= z + (z ^ (z >> 7)) * (z | 61UL);
  return z ^ (z >> 14);
}
