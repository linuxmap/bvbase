/*
 * Copyright (c) 2010 Mans Rullgard <mans@mansr.com>
 *
 * This file is part of BVbase.
 *
 * BVbase is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * BVbase is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BVbase; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef BVUTIL_ARM_INTMATH_H
#define BVUTIL_ARM_INTMATH_H

#include <stdint.h>

#include "config.h"
#include "libbvutil/attributes.h"

#if HAVE_INLINE_ASM

#if HAVE_ARMV6_INLINE

#define bv_clip_uint8 bv_clip_uint8_arm
static bv_always_inline bv_const unsigned bv_clip_uint8_arm(int a)
{
    unsigned x;
    __asm__ ("usat %0, #8,  %1" : "=r"(x) : "r"(a));
    return x;
}

#define bv_clip_int8 bv_clip_int8_arm
static bv_always_inline bv_const int bv_clip_int8_arm(int a)
{
    int x;
    __asm__ ("ssat %0, #8,  %1" : "=r"(x) : "r"(a));
    return x;
}

#define bv_clip_uint16 bv_clip_uint16_arm
static bv_always_inline bv_const unsigned bv_clip_uint16_arm(int a)
{
    unsigned x;
    __asm__ ("usat %0, #16, %1" : "=r"(x) : "r"(a));
    return x;
}

#define bv_clip_int16 bv_clip_int16_arm
static bv_always_inline bv_const int bv_clip_int16_arm(int a)
{
    int x;
    __asm__ ("ssat %0, #16, %1" : "=r"(x) : "r"(a));
    return x;
}

#define bv_clip_uintp2 bv_clip_uintp2_arm
static bv_always_inline bv_const unsigned bv_clip_uintp2_arm(int a, int p)
{
    unsigned x;
    __asm__ ("usat %0, %2, %1" : "=r"(x) : "r"(a), "i"(p));
    return x;
}

#define bv_sat_add32 bv_sat_add32_arm
static bv_always_inline int bv_sat_add32_arm(int a, int b)
{
    int r;
    __asm__ ("qadd %0, %1, %2" : "=r"(r) : "r"(a), "r"(b));
    return r;
}

#define bv_sat_dadd32 bv_sat_dadd32_arm
static bv_always_inline int bv_sat_dadd32_arm(int a, int b)
{
    int r;
    __asm__ ("qdadd %0, %1, %2" : "=r"(r) : "r"(a), "r"(b));
    return r;
}

#endif /* HAVE_ARMV6_INLINE */

#if HAVE_ASM_MOD_Q

#define bv_clipl_int32 bv_clipl_int32_arm
static bv_always_inline bv_const int32_t bv_clipl_int32_arm(int64_t a)
{
    int x, y;
    __asm__ ("adds   %1, %R2, %Q2, lsr #31  \n\t"
             "itet   ne                     \n\t"
             "mvnne  %1, #1<<31             \n\t"
             "moveq  %0, %Q2                \n\t"
             "eorne  %0, %1,  %R2, asr #31  \n\t"
             : "=r"(x), "=&r"(y) : "r"(a) : "cc");
    return x;
}

#endif /* HAVE_ASM_MOD_Q */

#endif /* HAVE_INLINE_ASM */

#endif /* BVUTIL_ARM_INTMATH_H */