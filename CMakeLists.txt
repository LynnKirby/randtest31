# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: 2020 Lynn Kirby

cmake_minimum_required(VERSION 3.16)

project(randtest31 C CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)

#
# Global settings.
#

if(WIN32)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
endif()

#
# Add sources.
#

add_subdirectory(src)
add_subdirectory(thirdparty)
