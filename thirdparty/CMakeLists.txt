# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: 2020 Lynn Kirby

add_subdirectory(PractRand)

#
# LittleIntPacker
# https://github.com/lemire/LittleIntPacker
#

add_library(
    LittleIntPacker
    STATIC
    LittleIntPacker/src/bitpacking32.c
    LittleIntPacker/src/util.c
)

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
    pcg-c/src/pcg-rngs-128.c
)

target_include_directories(pcg PUBLIC pcg-c/include)

#
# TestU01
# http://simul.iro.umontreal.ca/testu01/tu01.html
#
# Note, we assume you've built the library. Building is more complicated than
# compiling a list of files and replicating the build system in CMake would be
# annoying.
#

add_library(TestU01_mylib    STATIC IMPORTED GLOBAL)
add_library(TestU01_probdist STATIC IMPORTED GLOBAL)
add_library(TestU01_testu01  STATIC IMPORTED GLOBAL)

set(libdir ${CMAKE_CURRENT_LIST_DIR}/TestU01/lib/)

set_property(TARGET TestU01_mylib
             PROPERTY IMPORTED_LOCATION ${libdir}libmylib.a)
set_property(TARGET TestU01_probdist
             PROPERTY IMPORTED_LOCATION ${libdir}libprobdist.a)
set_property(TARGET TestU01_testu01
             PROPERTY IMPORTED_LOCATION ${libdir}libtestu01.a)

add_library(TestU01 INTERFACE)

target_link_libraries(
    TestU01
    INTERFACE
    TestU01_testu01
    TestU01_probdist
    TestU01_mylib
    m)

target_include_directories(TestU01 INTERFACE TestU01/include)
