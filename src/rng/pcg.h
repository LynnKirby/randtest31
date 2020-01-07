/*
 * SPDX-License-Identifier: CC0-1.0
 * SPDX-FileCopyrightText: 2020 Lynn Kirby
 * 
 * This file is only used to disable some warnings in pcg_variants.h that MSVC
 * produces.
 *
 * C4146: unary minus operator applied to unsigned type, result still unsigned
 * Unary negation of unsigned values is intentional.
 * 
 * C4244: conversion from 'type1' to 'type2', possible loss of data
 * This truncation is intentional.
 */

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4146)
#pragma warning(disable:4244)
#endif

#include "pcg_variants.h"

#ifdef _MSC_VER
#pragma warning(pop)
#endif