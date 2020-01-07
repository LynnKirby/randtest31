# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: 2020 Lynn Kirby

cmake_minimum_required(VERSION 3.16)

project(randtest31 C CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

#
# Handle build options.
#

# Enable warnings by default if we're a standalone project. Disable them if
# we're built as a subdirectory.
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(enable_warnings ON)
else()
    set(enable_warnings OFF)
endif()

set(RANDTEST31_ENABLE_WARNINGS ${enable_warnings} CACHE BOOL "Enable warnings.")

add_library(randtest31_build_config INTERFACE)
add_library(randtest31_enable_warning_options INTERFACE)
add_library(randtest31_disable_warning_options INTERFACE)

if(MSVC)
    target_compile_options(randtest31_enable_warning_options INTERFACE /W4)
    target_compile_options(randtest31_disable_warning_options INTERFACE /w)
else()
    target_compile_options(randtest31_enable_warning_options INTERFACE
                           -Wall -pedantic)
    target_compile_options(randtest31_disable_warning_options INTERFACE -w)
endif()

if(RANDTEST31_ENABLE_WARNINGS)
    target_link_libraries(randtest31_build_config INTERFACE
                          randtest31_enable_warning_options)
else()
    target_link_libraries(randtest31_build_config INTERFACE
                          randtest31_disable_warning_options)
endif()

#
# Global settings.
#

if(WIN32)
    target_compile_definitions(randtest31_build_config INTERFACE
                               _CRT_SECURE_NO_WARNINGS)
endif()

#
# Add sources.
#

add_subdirectory(src)
add_subdirectory(thirdparty)
