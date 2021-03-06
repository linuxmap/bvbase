/*
 * Copyright (c) 2000, 2001, 2002 Fabrice Bellard
 *
 * This file is part of BVBase.
 *
 * BVBase is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * BVBase is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BVBase; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef BVUTIL_CPU_H
#define BVUTIL_CPU_H

#include "attributes.h"

#define BV_CPU_FLAG_FORCE    0x80000000 /* force usage of selected flags (OR) */

    /* lower 16 bits - CPU features */
#define BV_CPU_FLAG_MMX          0x0001 ///< standard MMX
#define BV_CPU_FLAG_MMXEXT       0x0002 ///< SSE integer functions or AMD MMX ext
#define BV_CPU_FLAG_MMX2         0x0002 ///< SSE integer functions or AMD MMX ext
#define BV_CPU_FLAG_3DNOW        0x0004 ///< AMD 3DNOW
#define BV_CPU_FLAG_SSE          0x0008 ///< SSE functions
#define BV_CPU_FLAG_SSE2         0x0010 ///< PIV SSE2 functions
#define BV_CPU_FLAG_SSE2SLOW 0x40000000 ///< SSE2 supported, but usually not faster
                                        ///< than regular MMX/SSE (e.g. Core1)
#define BV_CPU_FLAG_3DNOWEXT     0x0020 ///< AMD 3DNowExt
#define BV_CPU_FLAG_SSE3         0x0040 ///< Prescott SSE3 functions
#define BV_CPU_FLAG_SSE3SLOW 0x20000000 ///< SSE3 supported, but usually not faster
                                        ///< than regular MMX/SSE (e.g. Core1)
#define BV_CPU_FLAG_SSSE3        0x0080 ///< Conroe SSSE3 functions
#define BV_CPU_FLAG_ATOM     0x10000000 ///< Atom processor, some SSSE3 instructions are slower
#define BV_CPU_FLAG_SSE4         0x0100 ///< Penryn SSE4.1 functions
#define BV_CPU_FLAG_SSE42        0x0200 ///< Nehalem SSE4.2 functions
#define BV_CPU_FLAG_AVX          0x4000 ///< AVX functions: requires OS support even if YMM registers aren't used
#define BV_CPU_FLAG_XOP          0x0400 ///< Bulldozer XOP functions
#define BV_CPU_FLAG_FMA4         0x0800 ///< Bulldozer FMA4 functions
// #if LIBBVUTIL_VERSION_MAJOR <52
#define BV_CPU_FLAG_CMOV      0x1001000 ///< supports cmov instruction
// #else
// #define BV_CPU_FLAG_CMOV         0x1000 ///< supports cmov instruction
// #endif
#define BV_CPU_FLAG_AVX2         0x8000 ///< AVX2 functions: requires OS support even if YMM registers aren't used
#define BV_CPU_FLAG_FMA3        0x10000 ///< Haswell FMA3 functions
#define BV_CPU_FLAG_BMI1        0x20000 ///< Bit Manipulation Instruction Set 1
#define BV_CPU_FLAG_BMI2        0x40000 ///< Bit Manipulation Instruction Set 2

#define BV_CPU_FLAG_ALTIVEC      0x0001 ///< standard

#define BV_CPU_FLAG_ARMV5TE      (1 << 0)
#define BV_CPU_FLAG_ARMV6        (1 << 1)
#define BV_CPU_FLAG_ARMV6T2      (1 << 2)
#define BV_CPU_FLAG_VFP          (1 << 3)
#define BV_CPU_FLAG_VFPV3        (1 << 4)
#define BV_CPU_FLAG_NEON         (1 << 5)
#define BV_CPU_FLAG_ARMV8        (1 << 6)
#define BV_CPU_FLAG_SETEND       (1 <<16)

/**
 * Return the flags which specify extensions supported by the CPU.
 * The returned value is affected by bv_force_cpu_flags() if that was used
 * before. So bv_get_cpu_flags() can easily be used in a application to
 * detect the enabled cpu flags.
 */
int bv_get_cpu_flags(void);

/**
 * Disables cpu detection and forces the specified flags.
 * -1 is a special case that disables forcing of specific flags.
 */
void bv_force_cpu_flags(int flags);

/**
 * Set a mask on flags returned by bv_get_cpu_flags().
 * This function is mainly useful for testing.
 * Please use bv_force_cpu_flags() and bv_get_cpu_flags() instead which are more flexible
 *
 * @warning this function is not thread safe.
 */
attribute_deprecated void bv_set_cpu_flags_mask(int mask);

/**
 * Parse CPU flags from a string.
 *
 * The returned flags contain the specified flags as well as related unspecified flags.
 *
 * This function exists only for compatibility with libav.
 * Please use bv_parse_cpu_caps() when possible.
 * @return a combination of BV_CPU_* flags, negative on error.
 */
attribute_deprecated
int bv_parse_cpu_flags(const char *s);

/**
 * Parse CPU caps from a string and update the given BV_CPU_* flags based on that.
 *
 * @return negative on error.
 */
int bv_parse_cpu_caps(unsigned *flags, const char *s);

/**
 * @return the number of logical CPU cores present.
 */
int bv_cpu_count(void);

#endif /* BVUTIL_CPU_H */
