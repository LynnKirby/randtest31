/*
 * SPDX-License-Identifier: LicenseRef-newlib
 * SPDX-FileCopyrightText: 2020 Lynn Kirby
 *
 * Modified from newlib newlib/libc/stdlib/rand_r.c
 */

#include <stdint.h>
#include <limits.h>

/* Pseudo-random generator based on Minimal Standard by
   Lewis, Goodman, and Miller in 1969.

   I[j+1] = a*I[j] (mod m)

   where a = 16807
         m = 2147483647

   Using Schrage's algorithm, a*I[j] (mod m) can be rewritten as:

     a*(I[j] mod q) - r*{I[j]/q}      if >= 0
     a*(I[j] mod q) - r*{I[j]/q} + m  otherwise

   where: {} denotes integer division
          q = {m/a} = 127773
          r = m (mod a) = 2836

   note that the seed value of 0 cannot be used in the calculation as
   it results in 0 itself
*/

uint32_t state;

void newlib_rand_r_init(uint32_t seed)
{
    state = seed;
}

uint32_t newlib_rand_r_next()
{
    unsigned *seed = &state;
    long k;
    long s = (long)(*seed);
    if (s == 0)
      s = 0x12345987;
    k = s / 127773;
    s = 16807 * (s - k * 127773) - 2836 * k;
    if (s < 0)
      s += 2147483647;
    (*seed) = (unsigned int)s;
    return (int)(s & INT32_MAX);
}
