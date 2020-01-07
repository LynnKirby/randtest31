# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: 2020 Lynn Kirby

add_subdirectory(PractRand)
add_subdirectory(TestU01)

#
# LittleIntPacker
# https://github.com/lemire/LittleIntPacker
#

add_library(
    LittleIntPacker
    STATIC
    LittleIntPacker/src/bitpacking32.c
    LittleIntPacker/src/util.c)

target_include_directories(LittleIntPacker PUBLIC LittleIntPacker/include)

#
# pcg-c
# https://github.com/imneme/pcg-c
#

add_library(
    pcg
    STATIC
    pcg-c/src/pcg-advance-8.c
    pcg-c/src/pcg-advance-16.c
    pcg-c/src/pcg-advance-32.c
    pcg-c/src/pcg-advance-64.c
    pcg-c/src/pcg-advance-128.c
    pcg-c/src/pcg-global-32.c
    pcg-c/src/pcg-global-64.c
    pcg-c/src/pcg-output-8.c
    pcg-c/src/pcg-output-16.c
    pcg-c/src/pcg-output-32.c
    pcg-c/src/pcg-output-64.c
    pcg-c/src/pcg-output-128.c
    pcg-c/src/pcg-rngs-8.c
    pcg-c/src/pcg-rngs-16.c
    pcg-c/src/pcg-rngs-32.c
    pcg-c/src/pcg-rngs-64.c
    pcg-c/src/pcg-rngs-128.c)

target_include_directories(pcg PUBLIC pcg-c/include)
target_link_libraries(pcg PRIVATE randtest31_disable_warning_options)