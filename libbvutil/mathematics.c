/*
 * Copyright (c) 2005-2012 Michael Niedermayer <michaelni@gmx.at>
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

/**
 * @file
 * miscellaneous math routines and tables
 */

#include <stdint.h>
#include <limits.h>

#include "mathematics.h"
#include "libbvutil/common.h"
#include "bvassert.h"
#include "version.h"

#if BV_API_BV_REVERSE
const uint8_t bv_reverse[256] = {
0x00,0x80,0x40,0xC0,0x20,0xA0,0x60,0xE0,0x10,0x90,0x50,0xD0,0x30,0xB0,0x70,0xF0,
0x08,0x88,0x48,0xC8,0x28,0xA8,0x68,0xE8,0x18,0x98,0x58,0xD8,0x38,0xB8,0x78,0xF8,
0x04,0x84,0x44,0xC4,0x24,0xA4,0x64,0xE4,0x14,0x94,0x54,0xD4,0x34,0xB4,0x74,0xF4,
0x0C,0x8C,0x4C,0xCC,0x2C,0xAC,0x6C,0xEC,0x1C,0x9C,0x5C,0xDC,0x3C,0xBC,0x7C,0xFC,
0x02,0x82,0x42,0xC2,0x22,0xA2,0x62,0xE2,0x12,0x92,0x52,0xD2,0x32,0xB2,0x72,0xF2,
0x0A,0x8A,0x4A,0xCA,0x2A,0xAA,0x6A,0xEA,0x1A,0x9A,0x5A,0xDA,0x3A,0xBA,0x7A,0xFA,
0x06,0x86,0x46,0xC6,0x26,0xA6,0x66,0xE6,0x16,0x96,0x56,0xD6,0x36,0xB6,0x76,0xF6,
0x0E,0x8E,0x4E,0xCE,0x2E,0xAE,0x6E,0xEE,0x1E,0x9E,0x5E,0xDE,0x3E,0xBE,0x7E,0xFE,
0x01,0x81,0x41,0xC1,0x21,0xA1,0x61,0xE1,0x11,0x91,0x51,0xD1,0x31,0xB1,0x71,0xF1,
0x09,0x89,0x49,0xC9,0x29,0xA9,0x69,0xE9,0x19,0x99,0x59,0xD9,0x39,0xB9,0x79,0xF9,
0x05,0x85,0x45,0xC5,0x25,0xA5,0x65,0xE5,0x15,0x95,0x55,0xD5,0x35,0xB5,0x75,0xF5,
0x0D,0x8D,0x4D,0xCD,0x2D,0xAD,0x6D,0xED,0x1D,0x9D,0x5D,0xDD,0x3D,0xBD,0x7D,0xFD,
0x03,0x83,0x43,0xC3,0x23,0xA3,0x63,0xE3,0x13,0x93,0x53,0xD3,0x33,0xB3,0x73,0xF3,
0x0B,0x8B,0x4B,0xCB,0x2B,0xAB,0x6B,0xEB,0x1B,0x9B,0x5B,0xDB,0x3B,0xBB,0x7B,0xFB,
0x07,0x87,0x47,0xC7,0x27,0xA7,0x67,0xE7,0x17,0x97,0x57,0xD7,0x37,0xB7,0x77,0xF7,
0x0F,0x8F,0x4F,0xCF,0x2F,0xAF,0x6F,0xEF,0x1F,0x9F,0x5F,0xDF,0x3F,0xBF,0x7F,0xFF,
};
#endif

int64_t bv_gcd(int64_t a, int64_t b)
{
    if (b)
        return bv_gcd(b, a % b);
    else
        return a;
}

int64_t bv_rescale_rnd(int64_t a, int64_t b, int64_t c, enum BVRounding rnd)
{
    int64_t r = 0;
    bv_assert2(c > 0);
    bv_assert2(b >=0);
    bv_assert2((unsigned)(rnd&~BV_ROUND_PASS_MINMAX)<=5 && (rnd&~BV_ROUND_PASS_MINMAX)!=4);

    if (c <= 0 || b < 0 || !((unsigned)(rnd&~BV_ROUND_PASS_MINMAX)<=5 && (rnd&~BV_ROUND_PASS_MINMAX)!=4))
        return INT64_MIN;

    if (rnd & BV_ROUND_PASS_MINMAX) {
        if (a == INT64_MIN || a == INT64_MAX)
            return a;
        rnd -= BV_ROUND_PASS_MINMAX;
    }

    if (a < 0 && a != INT64_MIN)
        return -bv_rescale_rnd(-a, b, c, rnd ^ ((rnd >> 1) & 1));

    if (rnd == BV_ROUND_NEAR_INF)
        r = c / 2;
    else if (rnd & 1)
        r = c - 1;

    if (b <= INT_MAX && c <= INT_MAX) {
        if (a <= INT_MAX)
            return (a * b + r) / c;
        else
            return a / c * b + (a % c * b + r) / c;
    } else {
#if 1
        uint64_t a0  = a & 0xFFFFFFFF;
        uint64_t a1  = a >> 32;
        uint64_t b0  = b & 0xFFFFFFFF;
        uint64_t b1  = b >> 32;
        uint64_t t1  = a0 * b1 + a1 * b0;
        uint64_t t1a = t1 << 32;
        int i;

        a0  = a0 * b0 + t1a;
        a1  = a1 * b1 + (t1 >> 32) + (a0 < t1a);
        a0 += r;
        a1 += a0 < r;

        for (i = 63; i >= 0; i--) {
            a1 += a1 + ((a0 >> i) & 1);
            t1 += t1;
            if (c <= a1) {
                a1 -= c;
                t1++;
            }
        }
        return t1;
    }
#else
        BVInteger ai;
        ai = bv_mul_i(bv_int2i(a), bv_int2i(b));
        ai = bv_add_i(ai, bv_int2i(r));

        return bv_i2int(bv_div_i(ai, bv_int2i(c)));
    }
#endif
}

int64_t bv_rescale(int64_t a, int64_t b, int64_t c)
{
    return bv_rescale_rnd(a, b, c, BV_ROUND_NEAR_INF);
}

int64_t bv_rescale_q_rnd(int64_t a, BVRational bq, BVRational cq,
                         enum BVRounding rnd)
{
    int64_t b = bq.num * (int64_t)cq.den;
    int64_t c = cq.num * (int64_t)bq.den;
    return bv_rescale_rnd(a, b, c, rnd);
}

int64_t bv_rescale_q(int64_t a, BVRational bq, BVRational cq)
{
    return bv_rescale_q_rnd(a, bq, cq, BV_ROUND_NEAR_INF);
}

int bv_compare_ts(int64_t ts_a, BVRational tb_a, int64_t ts_b, BVRational tb_b)
{
    int64_t a = tb_a.num * (int64_t)tb_b.den;
    int64_t b = tb_b.num * (int64_t)tb_a.den;
    if ((BBABS(ts_a)|a|BBABS(ts_b)|b) <= INT_MAX)
        return (ts_a*a > ts_b*b) - (ts_a*a < ts_b*b);
    if (bv_rescale_rnd(ts_a, a, b, BV_ROUND_DOWN) < ts_b)
        return -1;
    if (bv_rescale_rnd(ts_b, b, a, BV_ROUND_DOWN) < ts_a)
        return 1;
    return 0;
}

int64_t bv_compare_mod(uint64_t a, uint64_t b, uint64_t mod)
{
    int64_t c = (a - b) & (mod - 1);
    if (c > (mod >> 1))
        c -= mod;
    return c;
}

int64_t bv_rescale_delta(BVRational in_tb, int64_t in_ts,  BVRational fs_tb, int duration, int64_t *last, BVRational out_tb){
    int64_t a, b, this;

    bv_assert0(in_ts != BV_NOPTS_VALUE);
    bv_assert0(duration >= 0);

    if (*last == BV_NOPTS_VALUE || !duration || in_tb.num*(int64_t)out_tb.den <= out_tb.num*(int64_t)in_tb.den) {
simple_round:
        *last = bv_rescale_q(in_ts, in_tb, fs_tb) + duration;
        return bv_rescale_q(in_ts, in_tb, out_tb);
    }

    a =  bv_rescale_q_rnd(2*in_ts-1, in_tb, fs_tb, BV_ROUND_DOWN)   >>1;
    b = (bv_rescale_q_rnd(2*in_ts+1, in_tb, fs_tb, BV_ROUND_UP  )+1)>>1;
    if (*last < 2*a - b || *last > 2*b - a)
        goto simple_round;

    this = bv_clip64(*last, a, b);
    *last = this + duration;

    return bv_rescale_q(this, fs_tb, out_tb);
}

int64_t bv_add_stable(BVRational ts_tb, int64_t ts, BVRational inc_tb, int64_t inc)
{
    int64_t m, d;

    if (inc != 1)
        inc_tb = bv_mul_q(inc_tb, (BVRational) {inc, 1});

    m = inc_tb.num * (int64_t)ts_tb.den;
    d = inc_tb.den * (int64_t)ts_tb.num;

    if (m % d == 0)
        return ts + m / d;
    if (m < d)
        return ts;

    {
        int64_t old = bv_rescale_q(ts, ts_tb, inc_tb);
        int64_t old_ts = bv_rescale_q(old, inc_tb, ts_tb);
        return bv_rescale_q(old + 1, inc_tb, ts_tb) + (ts - old_ts);
    }
}
